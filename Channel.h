// channel.h
#pragma once
#include <string>
#include "socket.h"  // Base Socket class (and where TCPSocket, UDPSocket come from)

class Channel {
protected:
    Socket* channelSocket;  // polymorphic socket pointer (TCP or UDP)

public:
    Channel(Socket* sock) : channelSocket(sock) {}
    virtual ~Channel() { delete channelSocket; }

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void send(const std::string& message) = 0;
    virtual void receive() = 0;
};
