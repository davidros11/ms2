#include "Server_side.h"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <exception>
using namespace std;
using namespace server_side;
FileCacheManager& FileCacheManager::getInstance() {
    static FileCacheManager cm;
    return cm;
}
void FileCacheManager::store(const string& problem, const char *solution) {
    ofstream out;
    string tempName = problem + "a.txt";
    out.open(tempName);
    if(!out) {
        return;
    }
    out.write(solution, strlen(solution));
    string realName = problem + ".txt";
    rename(tempName.c_str(), realName.c_str());
    out.close();
}
string FileCacheManager::load(const string& problem) {
    ifstream in;
    string fileName = problem + ".txt";
    in.open(fileName);
    if(!in) {
        return "";
    }
    string objString((std::istreambuf_iterator<char>(in)),
                std::istreambuf_iterator<char>());
    in.close();
    return objString;
}
/**
 * Seperates a string into tokens.
 * @param str - the string
 * @param seps - the characters to seperate by
 * @param omit - subset of seps that won't be included as tokens
 * @return - the resulting token list
 */
vector<string> lexer(const string& str, const vector<string>& seps, const vector<string>& omit) {
    auto lex = vector<string>();
    int len = str.length();
    int last = 0;
    int cur = 0;
    bool found = false;
    bool shouldKeep = true;
    while (cur < len) {
        // don't seperate what's in quotation marks if quotation marks are expected
        for(const string& s : seps) {
            if(s == str.substr(cur, s.length())) {
                string token = str.substr(last, cur - last);
                if(!token.empty()) {
                    lex.push_back(token);
                }
                for(const string& o : omit) {
                    if(s == o) {
                        shouldKeep = false;
                    }
                }
                if(shouldKeep) {
                    lex.push_back(s);

                } else {
                    shouldKeep = true;
                }
                cur += s.length();
                last = cur;
                found = true;
                break;
            }
        }
        if(found) {
            found = false;

        } else {
            cur++;
        }
    }
    string final = str.substr(last, cur - last);
    if(!final.empty()) {
        lex.push_back(final);
    }
    return lex;
}
/**
 * Hashes the matrix into a string
 * @param mat - the matrix
 * @return - the hash string
 */
string hashMatrix(const Matrix& mat) {
    auto init = mat.getInitialState();
    auto end = mat.getGoalState();
    auto vec = mat.getVec();
    // inserts the initial and goal positions, the matrix legnth, and the sum of all cells times their number.
    string hash = init->getState() + "." + end->getState() + "." + to_string(vec.size()) + ".";
    delete init;
    delete end;
    double sum = 0;
    int count = 1;
    for(const auto& row : vec) {
        for(double num : row) {
            sum += num*count;
            count++;
        }
    }
    hash += to_string(sum);
    return hash;
}
/**
 * Converts a string into a matrix.
 * @param input - a string representing a matrix.
 * @return - the resulting matrix
 */
Matrix getMatrix(const string &input) {
    auto lex = lexer(input, { "\n" , ",", " " }, {",", " "});
    vector<vector<double>> mat;
    mat.resize(1);
    int pos = 0;
    // getting the first line, to see the length of the matrix
    while(lex.at(pos) != "\n") {
        mat.at(0).push_back(stod(lex.at(pos)));
        ++pos;
    }
    // continuing reading lines until reaching the end of the matrix
    int len = pos++;
    mat.resize(len);
    for(int row = 1; row < len; row++) {
        while(lex.at(pos) != "\n") {
            string v = lex.at(pos);
            double val = stod(v);
            mat.at(row).push_back(val);
            ++pos;
        }
        ++pos;
    }
    auto a = lex.at(pos);
    // adding the initial and end points
    auto init = pair<int, int>(stoi(lex.at(pos)), stoi(lex.at(pos+1)));
    auto goal = pair<int, int>(stoi(lex.at(pos+3)), stoi(lex.at(pos+4)));
    return Matrix(mat, init, goal);
}
MyClientHandler::MyClientHandler(CacheManager *cache, Solver<Searchable*, string> *s) {
    cacheManager = cache;
   solver = s;
}
/**
 * Reads a matrix from the client
 * @param socket - the socket with the client
 * @return - the matrix in string form.
 */
string readMatrix(int socket) {
    char buffer[1025] = {0};
    string input;
    int pos;
    // reading until find 'e' in the input
    while(true) {
        int bytesRead = read(socket, buffer, 1024);
        if(bytesRead < 1) {
            continue;
        }
        string temp(buffer);
        if((pos = temp.find('e')) != -1) {
            input += temp.substr(0, pos);
            break;
        }
        input += temp;
    }
    return input;
}
void MyClientHandler::handleClient(int sockFd) {
    string input = readMatrix(sockFd);
    Matrix matrix = getMatrix(input);
    string hashed = hashMatrix(matrix);
    string answer = cacheManager->load(hashed);
    if (answer.empty()) {
        string output = solver->solve(&matrix);
        cacheManager->store(hashed, output.c_str());
        write(sockFd, output.c_str(), output.length());

    } else {
        write(sockFd, answer.c_str(), answer.length());
    }
    close(sockFd);
}
void MyParallelServer::open(int port, ClientHandler* handlers[CLIENT_LIMIT]) {
    struct sockaddr_in address;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    // preparing socket
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener == -1) {
        perror("socket");
        return;
    }
    if(bind(listener, (sockaddr *)&address, sizeof(address)) == -1) {
        perror("bind");
        close(listener);
        return;
    }
    if(listen(listener, CLIENT_LIMIT) == -1) {
        // listen failed
        perror("listener");
        close(listener);
        return;
    }
    int currentThread = 0;
    struct timeval tv;
    tv.tv_usec = 0;
    while(true) {
        tv.tv_sec = 120;
        setsockopt(listener, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
        int conn = accept(listener, (sockaddr *)&address, (socklen_t *)&address);
        if(conn == -1) {
            close(listener);
            cout << "closing" << endl;
            stop();
            return;
        }
        tv.tv_sec = 0;
        setsockopt(listener, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
        if(clientThreads[currentThread].joinable()) {
            clientThreads[currentThread].join();
        }
        clientThreads[currentThread] = thread(&ClientHandler::handleClient, handlers[currentThread], conn);

        if(currentThread == CLIENT_LIMIT - 1) {
            currentThread = 0;
        }
        else {
            ++currentThread;
        }
    }

}
void MyParallelServer::stop() {
    for(thread &th : clientThreads) {
        if(th.joinable()) {
            th.join();
        }
    }
}