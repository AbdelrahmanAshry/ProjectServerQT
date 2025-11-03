#pragma once
#include "Channel.h"
#include <thread>
#include <atomic>

class ClientChannel : public Channel {
private:
    std::atomic<bool> running{false};
    int currentThreshold = 70;

    void communicationLoop();
    void handleMessage(const std::string& msg);

public:
    using Channel::Channel;//same constructors as parent

    void start() override;
    void stop() override;
    void send(const std::string& message) override;
    void receive() override;
};
