#include "tcp_socket.h"
#include "udp_socket.h"
#include "Server_Channel.h"
#include "Client_Channel.h"
#include <iostream>
#include <memory>

int main() {
    bool useTCP = true;
    std::cout << "Run TCP server? (1=yes, 0=UDP): ";
    std::cin >> useTCP;

    std::unique_ptr<Socket> sock;
    if (useTCP)
        sock = std::make_unique<TCPSocket>();
    else
        sock = std::make_unique<UDPSocket>();

    ServerChannel server(std::move(sock));
    server.start();

    std::cout << "Press Enter to stop server..." << std::endl;
    std::cin.get(); std::cin.get();
    server.stop();
    return 0;
}
