#include <iostream>
#include "Boot.h"
#define DEF_PORT 5400
int main(int argc, char *argv[]) {
    int port = DEF_PORT;
    if(argc >= 2) {
        port = stoi(argv[0]);
    }
    boot::Main m;
    m.main(port);
    cout << "done\n";
    return 0;
}
