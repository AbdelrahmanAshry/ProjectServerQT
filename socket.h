#ifndef SOCKET_H
#define SOCKET_H
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class Socket {
public:
    virtual ~Socket() {};
    virtual void waitForConnect(int port) = 0;
    virtual void connect(const std::string &host, int port) = 0;
    virtual unsigned int send(const std::string &msg, const sockaddr_in* dest = nullptr) = 0;
    virtual unsigned int receive(std::string &out, sockaddr_in* src = nullptr) = 0;
    virtual void shutdown() = 0;
    virtual bool isTCP() const = 0;
};
#endif
