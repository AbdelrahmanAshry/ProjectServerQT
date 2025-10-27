#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H
#include "socket.h"

class TCPSocket : public Socket
{
private:
    int connFd{-1}; // client or accepted socket
    int listenFd{-1};
public:
    TCPSocket();
    ~TCPSocket();
    void waitForConnect(int port) = 0;
    void connect(const std::string &host, int port) = 0;
    unsigned int send(const std::string &msg, const sockaddr_in* dest = nullptr) = 0;
    unsigned int receive(std::string &out, sockaddr_in* src = nullptr) = 0;
    void shutdown() = 0;
    bool isTCP() const = 0;
};



#endif