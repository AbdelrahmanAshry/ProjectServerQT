// channel.h
#pragma once
#include <string>
#include "Socket.h"  // Base Socket class (and where TCPSocket, UDPSocket come from)

class Channel {
protected:
    Socket* channelSocket;  // polymorphic socket pointer (TCP or UDP)
    bool ownsSocket{true};  // if true Channel destructor will delete channelSocket

public:
    // takeOwnership defaults to true to preserve previous behavior
    Channel(Socket* sock, bool takeOwnership = true)
        : channelSocket(sock), ownsSocket(takeOwnership) {}
    virtual ~Channel() { if (ownsSocket) delete channelSocket; }

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void send(const std::string& message) = 0;
    virtual void receive() = 0;
};
