#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H
#include "Socket.h"

class TCPSocket : public Socket
{
private:
    int connFd{-1}; // client or accepted socket
    int listenFd{-1};
public:
    TCPSocket();
    ~TCPSocket();

    // Rule of Three: copy ctor + copy assignment (destructor already present)
    TCPSocket(const TCPSocket& other);
    TCPSocket& operator=(const TCPSocket& other);

    // concrete overrides (implemented in tcp_socket.cpp)
    void waitForConnect(int port) override;
    void connect(const std::string &host, int port) override;
    unsigned int send(const std::string &msg, const sockaddr_in* dest = nullptr) override;
    unsigned int receive(std::string &out, sockaddr_in* src = nullptr) override;
    void shutdown() override;
    bool isTCP() const override { return true; }
};

#endif