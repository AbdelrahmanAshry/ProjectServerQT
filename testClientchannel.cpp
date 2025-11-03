#include "Client_Channel.h"
#include "tcp_socket.h"
#include "udp_socket.h"
#include <iostream>
#include <memory>

int main() {
    bool useTCP = true;
    std::cout << "Run TCP client? (1=yes, 0=UDP): ";
    std::cin >> useTCP;

    std::unique_ptr<Socket> sock;
    if (useTCP)
        sock = std::make_unique<TCPSocket>();
    else
        sock = std::make_unique<UDPSocket>();

    ClientChannel client(std::move(sock));
    client.start();

    std::cout << "Press Enter to stop client..." << std::endl;
    std::cin.get(); std::cin.get();
    client.stop();
    return 0;
}
