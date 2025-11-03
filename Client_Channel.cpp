#include "Client_Channel.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <cstdlib> // for rand()

void ClientChannel::start() {
    running = true;
    std::cout << "[ClientChannel] Connecting to server..." << std::endl;
    channelSocket->connectToServer("127.0.0.1", 8081);
if (!channelSocket->isConnectionOriented()) {
    std::string initMsg=" IDEAL";
    channelSocket->send(initMsg);  // UDP bootstrap
    std::cout << "[ClientChannel] Remote endpoint set.\n";
}
    std::thread(&ClientChannel::communicationLoop, this).detach();
}

void ClientChannel::stop() {
    running = false;
    channelSocket->shutdown();
    std::cout << "[ClientChannel] Disconnected." << std::endl;
}

void ClientChannel::send(const std::string& message) {
    channelSocket->send(message);
}

void ClientChannel::receive() {
    std::string msg;
    unsigned int len = channelSocket->receive(msg);
    if (len == 0) return;

    std::cout << "[ClientChannel] Received: " << msg << std::endl;
    handleMessage(msg);
}

void ClientChannel::handleMessage(const std::string& msg) {
    if (msg.rfind("set threshold", 0) == 0) {
        std::istringstream iss(msg);
        std::string cmd;
        int newThreshold;
        iss >> cmd >> cmd >> newThreshold;
        currentThreshold = newThreshold;
        std::cout << "[Client] Threshold updated to " << newThreshold << "\n";
    } else if (msg == "get temp") {
        int temp = 50 + rand() % 40;
        send("temperature " + std::to_string(temp));
    }
}

void ClientChannel::communicationLoop() {
    while (running) {
        receive();
    }
}
