// tcp_socket.cpp
#include "tcp_socket.h"
#include <iostream>

using boost::asio::ip::tcp;

TCPSocket::TCPSocket()
    : socket(std::make_unique<tcp::socket>(io)) {}

TCPSocket::~TCPSocket() {
    shutdown();
}
//bool TCPSocket::isConnectionOriented(){    return true; }

void TCPSocket::waitForConnect(int port) {
    try {
        tcp::endpoint endpoint(tcp::v4(), port);
        acceptor = std::make_unique<tcp::acceptor>(io, endpoint);
        std::cout << "[TCP] Waiting for client on port " << port << "...\n";
        acceptor->accept(*socket);
        std::cout << "[TCP] Client connected!\n";
    } catch (std::exception& e) {
        std::cerr << "[TCP Error] " << e.what() << std::endl;
    }
}

void TCPSocket::connectToServer(const std::string& host, int port) {
    try {
        tcp::resolver resolver(io);
        auto endpoints = resolver.resolve(host, std::to_string(port));
        boost::asio::connect(*socket, endpoints);
        std::cout << "[TCP] Connected to " << host << ":" << port << "\n";
    } catch (std::exception& e) {
        std::cerr << "[TCP Error] " << e.what() << std::endl;
    }
}

unsigned int TCPSocket::send(const std::string& msg) {
    try {
        return boost::asio::write(*socket, boost::asio::buffer(msg));
    } catch (std::exception& e) {
        std::cerr << "[TCP Send Error] " << e.what() << std::endl;
        return 0;
    }
}

unsigned int TCPSocket::receive(std::string& out) {
    try {
        char data[1024];
        size_t len = socket->read_some(boost::asio::buffer(data));
        out.assign(data, len);
        std::cout << "[TCP Received] " << out << std::endl;
        return static_cast<unsigned int>(len);
    } catch (std::exception& e) {
        std::cerr << "[TCP Receive Error] " << e.what() << std::endl;
        return 0;
    }
}

void TCPSocket::shutdown() {
    try {
        if (socket && socket->is_open()) {
            socket->close();
            std::cout << "[TCP] Socket closed.\n";
        }
        if (acceptor && acceptor->is_open()) {
            acceptor->close();
        }
    } catch (...) {}
}
