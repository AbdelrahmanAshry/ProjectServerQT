// server_channel.cpp
#include "Server_Channel.h"
#include <iostream>

ServerChannel::ServerChannel(Socket* sock)
    : Channel(sock) {}

void ServerChannel::start() {
    std::cout << "[Server] Starting..." << std::endl;
    channelSocket->waitForConnect();
}

void ServerChannel::stop() {
    std::cout << "[Server] Stopping..." << std::endl;
    channelSocket->shutdown();
}

void ServerChannel::send(const std::string& message) {
    channelSocket->send(message);
}

void ServerChannel::receive() {
    channelSocket->receive();
}
