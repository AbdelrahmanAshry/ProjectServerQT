// udp_socket.cpp
#include "udp_socket.h"
#include <iostream>

using boost::asio::ip::udp;

UDPSocket::UDPSocket()
    : socket(std::make_unique<udp::socket>(io)) {}

//UDPSocket::UDPSocket(boost::asio::ip::udp::socket &fola)
//    : socket(fola) {}
UDPSocket::~UDPSocket() {
    shutdown();
}
//bool UDPSocket::isConnectionOriented() { return false; }

void UDPSocket::waitForConnect(int port) {
    try {
        udp::endpoint endpoint(udp::v4(), port);
        socket->open(udp::v4());
        socket->bind(endpoint);
        std::cout << "[UDP] Listening on port " << port << "...\n";
        
    } catch (std::exception& e) {
        std::cerr << "[UDP Error] " << e.what() << std::endl;
    }
}

void UDPSocket::connectToServer(const std::string& host, int port) {
    udp::resolver resolver(io);
    auto endpoints = resolver.resolve(udp::v4(), host, std::to_string(port));
    remote = *endpoints.begin();
    remoteSet = true;
    socket->open(udp::v4());
    std::cout << "[UDP] Ready to send to " << host << ":" << port << "\n";
}

unsigned int UDPSocket::send(const std::string& msg) {
    try {
        return socket->send_to(boost::asio::buffer(msg), remote);
    } catch (std::exception& e) {
        std::cerr << "[UDP Send Error] " << e.what() << std::endl;
        return 0;
    }
}

unsigned int UDPSocket::receive(std::string& out) {
    try {
        char data[1024];
        udp::endpoint sender;
        size_t len = socket->receive_from(boost::asio::buffer(data), sender);
        out.assign(data, len);
        remote =sender;
        remoteSet = true;
        std::cout << "[UDP Received] " << out << std::endl;
        return static_cast<unsigned int>(len);
    } catch (std::exception& e) {
        std::cerr << "[UDP Receive Error] " << e.what() << std::endl;
        return 0;
    }
}

void UDPSocket::shutdown() {
    try {
        if (socket && socket->is_open()) {
            socket->close();
            std::cout << "[UDP] Socket closed.\n";
        }
    } catch (...) {}
}
