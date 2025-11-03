// server_channel.cpp
#include "Server_Channel.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib> // for rand()

// void ServerChannel::start() {
//     running = true;
//     std::cout << "[ServerChannel] Starting server..." << std::endl;
//     channelSocket->waitForConnect(8081);
     
// if (!channelSocket->isConnectionOriented()) {
//     std::cout << "[ServerChannel] Waiting for message" << std::endl;
    
//     std::string initMsg;
//     channelSocket->receive(initMsg);  // UDP bootstrap
//     std::cout << "[ServerChannel] Remote endpoint set.\n";
// }

//     std::thread(&ServerChannel::communicationLoop, this).detach();
// }

void ServerChannel::start() {
    running = true;
    std::cout << "[ServerChannel] Starting multi-protocol server..." << std::endl;
    this.threshold = 50.0;
    this.lastThreshold = this.threshold;
    // TCP listener
    if (tcpSocket) {
        std::thread([this]() {
            try {
                tcpSocket->waitForConnect(8081);
                std::cout << "[TCP] Client connected!\n";
                communicationLoop(tcpSocket.get(), "TCP");
            } catch (std::exception& e) {
                std::cerr << "[TCP Error] " << e.what() << std::endl;
            }
        }).detach();
    }

    // UDP listener
    if (udpSocket) {
        std::thread([this]() {
            try {
                udpSocket->waitForConnect(8082);
                std::cout << "[UDP] Listening for packets...\n";

                std::string firstMsg;
                udpSocket->receive(firstMsg); // waits until client sends first packet
                std::cout << "[UDP] Initial packet: " << firstMsg << std::endl;

                communicationLoop(udpSocket.get(), "UDP");
            } catch (std::exception& e) {
                std::cerr << "[UDP Error] " << e.what() << std::endl;
            }
        }).detach();
    }
}
void ServerChannel::stop() {
    running = false;
    channelSocket->shutdown();
    std::cout << "[ServerChannel] Stopped." << std::endl;
}

void ServerChannel::send(const std::string& message) {
    channelSocket->send(message);
}

void ServerChannel::receive() {
    std::string msg;
    channelSocket->receive(msg);
    std::cout << "[ServerChannel] Received: " << msg << std::endl;
}

void ServerChannel::communicationLoop() {
    // Send initial threshold command
    send("set threshold " + std::to_string(currentThreshold));

    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Random threshold change simulation
        if (rand() % 10 == 0) {
            currentThreshold = 60 + rand() % 20;
        }

        if (currentThreshold != lastThreshold) {
            send("set threshold " + std::to_string(currentThreshold));
            lastThreshold = currentThreshold;
        } else {
            send("get temp");
        }

        receive();
    }
}
