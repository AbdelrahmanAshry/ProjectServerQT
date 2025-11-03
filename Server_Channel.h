#pragma once
#include "Channel.h"
#include <thread>
#include <atomic>

class ServerChannel : public Channel {
private:
    std::atomic<bool> running{false};
    int currentThreshold = 70;
    int lastThreshold = 70;
    enum class State { IDLE, LISTENING, RUNNING, STOPPED };
    void communicationLoop();

public:
    using Channel::Channel;

    void start() override;
    void stop() override;
    void send(const std::string& message) override;
    void receive() override;
};
