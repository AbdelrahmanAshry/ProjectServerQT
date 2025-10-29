#include <iostream>
#include "test_udp_socket.h"

int main() {
    try {
        run_all_socket_tests();
        return 0;
    } catch (...) {
        std::cerr << "Unexpected exception running socket tests\n";
        return 2;
    }
}