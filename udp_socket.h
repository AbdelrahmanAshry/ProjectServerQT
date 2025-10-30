#ifndef UDP_SOCKET_H
#define UDP_SOCKET_H
#include "Socket.h"

class UDPSocket : public Socket
{
private:
    int connFd{-1}; // client or accepted socket
    int listenFd{-1};
public:
    UDPSocket();
    ~UDPSocket();

    // Rule of Three: copy ctor + copy assignment
    UDPSocket(const UDPSocket& other);
    UDPSocket& operator=(const UDPSocket& other);

    // concrete overrides (implemented in udp_socket.cpp)
    void waitForConnect(int port) override;
    void connect(const std::string &host, int port) override;
    unsigned int send(const std::string &msg, const sockaddr_in* dest = nullptr) override;
    unsigned int receive(std::string &out, sockaddr_in* src = nullptr) override;
    void shutdown() override;
    bool isTCP() const override { return false; }
};

#endif