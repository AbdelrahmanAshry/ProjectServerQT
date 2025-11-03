// channel.h
#pragma once
#include <string>
#include "Socket.h"  // Base Socket class (and where TCPSocket, UDPSocket come from)
#include <memory>

class Channel {
protected:
  //  Socket* channelSocket;  // polymorphic socket pointer (TCP or UDP)
  //  bool ownsSocket{true};  // if true Channel destructor will delete channelSocket
    std::unique_ptr<Socket> channelSocket; // either TCPSocket or UDPSocket unique pointer  better

public:
    // takeOwnership defaults to true to preserve previous behavior
    Channel(std::unique_ptr<Socket> sock) : channelSocket(std::move(sock)) {}
    virtual ~Channel() = default;// no need for destructor
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void send(const std::string& message) = 0;
    virtual void receive() = 0;
};
