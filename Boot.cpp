#include "Boot.h"
#include "Server_side.h"
#include "Searcher.h"
#include <iostream>


void boot::Main::main(int port) {
    // creating client handler array
    AStar searchers[CLIENT_LIMIT];
    server_side::SearchSolver *solvers[CLIENT_LIMIT];
    auto manager = server_side::FileCacheManager::getInstance();
    server_side::ClientHandler* handlers[CLIENT_LIMIT];
    for(int i = 0; i < CLIENT_LIMIT; i++) {
        solvers[i] = new server_side::SearchSolver(&searchers[i]);
        handlers[i] = new server_side::MyClientHandler(&manager, solvers[i]);
    }
    // opening server
    server_side::MyParallelServer server;
    server.open(port, handlers);
    for(int i = 0; i < CLIENT_LIMIT; i++) {
        delete solvers[i];
        delete handlers[i];
    }
}