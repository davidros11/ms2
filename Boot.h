//
// Created by david on 24/01/2020.
//

#ifndef PROJ2_BOOT_H
#define PROJ2_BOOT_H
#include <thread>
#include <mutex>
using namespace std;
namespace boot {
    static mutex printLock;
    void print(const string &str);
    void tester(int port);
    class Main {
    public:
        /**
         * opens the server.
         * @param port - port to listen with
         */
        void main(int port);
    };
}
#endif //PROJ2_BOOT_H
