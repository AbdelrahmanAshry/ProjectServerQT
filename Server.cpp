#include "Server_Channel.h"
#include "socket.h"
#include "tcp_socket.h"
#include "udp_socket.h"
#include <csignal>
#include <thread>
#include <chrono>
#include <iostream>

static ServerChannel* g_server_ptr = nullptr;

static void handle_sigint(int) {
    if (g_server_ptr) g_server_ptr->stop();
}

int main() {
    try {
        int choice = 1;
        std::cout << "Choose socket type (1=TCP, 2=UDP) [1]: ";
        if (!(std::cin >> choice)) choice = 1;

        int port = 8080;
        std::cout << "Port (default 8080): ";
        if (!(std::cin >> port)) port = 8080;

        // use static socket instances (no dynamic allocation)
        static TCPSocket tcpSock;
        static UDPSocket udpSock;

        Socket* sock = nullptr;
        bool takeOwnership = false; // we created static objects; Channel should NOT delete them
        if (choice == 1) {
            sock = &tcpSock;
        } else {
            sock = &udpSock;
        }

        // construct ServerChannel with ownership=false so Channel won't delete static socket
        ServerChannel server(sock, /*via Channel ctor*/ takeOwnership);
        server.setPort(port);

        // install Ctrl+C handler to stop gracefully
        g_server_ptr = &server; // polymorphic pointer
        std::signal(SIGINT, handle_sigint);

        server.start();
        std::cout << "[Main] Server started on port " << port << ". Press Ctrl+C to stop.\n";
        std::cout << "[Note] server sockets bind to INADDR_ANY (all local interfaces). Use 127.0.0.1 to connect from localhost.\n";

        // Single-threaded loop: call tick() every 1 second
        while (server.getState() == ServerChannel::RUNNING) {
            server.tick();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        server.stop();
        std::cout << "[Main] Server stopped.\n";
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return 2;
    }
}
