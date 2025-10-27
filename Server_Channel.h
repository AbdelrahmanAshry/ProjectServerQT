// server_channel.h
#pragma once
#include "Channel.h"
#include <iostream>

class ServerChannel : public Channel {
public:
    using Channel::Channel; // Inherit constructor

    void start();
    void stop();
    void send(const std::string& message) ;
    void receive();
};
