// server_channel.h
#pragma once
#include "Channel.h"
#include <string>

class ServerChannel : public Channel {
public:
    using Channel::Channel; // Inherit constructor

    void start() override;
    void stop() override;
    void send(const std::string& message) override;
    void receive() override;

    // New single-threaded poll method: call this every 1s (or from test) to process incoming messages.
    void tick();

    // configure the port to use when starting
    void setPort(int port) { bindPort = port; }

    // inspect internal state for tests
    enum State { IDLE, RUNNING, STOPPED };
    State getState() const { return state; }
    int getThreshold() const { return threshold; }

private:
    State state{IDLE};
    int bindPort{0};
    int threshold{0};
    double lastTemp{25.0};

    // parse a single received line and produce a reply (empty = no reply)
    std::string handleCommand(const std::string& cmd);
};
