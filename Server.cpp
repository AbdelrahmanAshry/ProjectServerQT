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

        Socket* sock = nullptr;
        if (choice == 1) {
            sock = new TCPSocket();
        } else {
            sock = new UDPSocket();
        }

        ServerChannel server(sock); // Channel takes ownership of sock
        server.setPort(port);

        // install Ctrl+C handler to stop gracefully
        g_server_ptr = &server;
        std::signal(SIGINT, handle_sigint);

        server.start();
        std::cout << "[Main] Server started on port " << port << ". Press Ctrl+C to stop.\n";

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
