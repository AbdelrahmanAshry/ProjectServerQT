// Client_Channel.h
#pragma once
#include "Channel.h"
#include <iostream>

class ClientChannel : public Channel {
public:
    using Channel::Channel; // Inherit constructor

    void start();
    void stop();
    void send(const std::string& message) ;
    void receive();
};
