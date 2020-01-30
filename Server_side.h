#ifndef PROJ2_SERVER_SIDE_H
#define PROJ2_SERVER_SIDE_H
using namespace std;
#include <iostream>
#include <fstream>
#include "Searchable.h"
#include "Searcher.h"
#include <thread>
// amount of clients the server can receive at once
#define CLIENT_LIMIT 10
namespace server_side {
    // stores solutions
    class CacheManager {
    public:
        /**
         * Stores a solution to a problem.
         * @param problem - the problem in string form
         * @param solution - the solution in string form
         */
        virtual void store(const string& problem, const char *solution) = 0;
        /**
         * load a solution.
         * @param problem in string form
         * @return - the solution in string form
         */
        virtual string load(const string& problem) = 0;
        /**
         * Destructor.
         */
        virtual ~CacheManager() = default;
    };
    // stores solutions in files. follows singleton pattern.
    class FileCacheManager: public CacheManager {
    public:
        /**
         * Get an instance of the class.
         * @return - the instnace
         */
        static FileCacheManager& getInstance();
        /**
         * Stores a solution in a file
         * @param problem - problem in string form
         * @param solution - solution in string form
         */
        void store(const string& problem, const char *solution) override;
        /**
         * load a solution from a file.
         * @param problem in string form
         * @return - the solution in string form
         */
        string load(const string& problem) override;
    };
    // solves problems for the client
    template <typename Problem, typename Solution>
    class Solver {
    public:
        /**
         * solves a problem and returns the solution.
         * @param prob - the problem
         * @return - the solution
         */
        virtual Solution solve(Problem prob) = 0;
        /**
         * Destructor.
         */
        virtual ~Solver() = default;
    };
    // objects adapter for searcher to solver.
    class SearchSolver :  public Solver<Searchable*, string> {
    private:
        Searcher *searcher;
    public:
        /**
         * Constructor. gets Searcher object
         * @param search - the searcher object
         */
        explicit SearchSolver(Searcher *search) { searcher = search; }
        /**
         * returns the path result given from the searcher
         * @param prob
         * @return
         */
        string solve(Searchable* prob) override {
            return searcher->search(prob) + '\n';
        }
    };
    class ClientHandler {
    public:
        /**
         * solves a problem for a client, saves the solution and sends it.
         * @param sockFd - a socket that connects to the client
         */
        virtual void handleClient(int sockFd) = 0;
        /**
         * Destructor.
         */
        virtual ~ClientHandler() = default;
    };
    class MyClientHandler : public ClientHandler {
    private:
        Solver<Searchable*, string> *solver;
        CacheManager *cacheManager;
    public:
        /**
         * Constructor. Takes solver and cache manager.
         * @param cache - cache manager
         * @param s - solver
         */
        MyClientHandler(CacheManager *cache, Solver<Searchable*, string> *s);
        /**
        * solves a search problem for a client, saves the solution and sends it.
        * @param sockFd - a socket that connects to the client
        */
        void handleClient(int sockFd) override;
    };
    class Server {
    public:
        /**
         * Opens a server.
         * @param port - port to listen with
         * @param handlers - array of client handlers to deal with clients
         */
        virtual void open(int port, ClientHandler* handlers[CLIENT_LIMIT]) = 0;
        /**
         * Stops server.
         */
        virtual void stop() = 0;
        /**
         * Destructor.
         */
        virtual ~Server() = default;
    };
    class MyParallelServer : public Server {
    private:
        thread clientThreads[CLIENT_LIMIT];
    public:
        /**
         * Opens a server.
         * @param port - port to listen with
         * @param handlers - array of client handlers to deal with clients
         */
        void open(int port, ClientHandler* handlers[CLIENT_LIMIT]) override;
        /**
        * Stops server.
        */
        void stop() override;
    };
}

#endif //PROJ2_SERVER_SIDE_H
