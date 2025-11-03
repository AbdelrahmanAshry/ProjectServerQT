#ifndef SOCKET_H
#define SOCKET_H
#include <string>

class Socket {
public:
   //Socket(); virtual class for polymerphism only 
    virtual ~Socket() = default;
    virtual void waitForConnect(int port) = 0;
    virtual void connectToServer(const std::string& host, int port) = 0;
    virtual unsigned int send(const std::string& msg) = 0;
    virtual unsigned int receive(std::string& out) = 0;
    virtual void shutdown() = 0;
//    virtual bool isConnectionOriented() ;//is it TCP or not 
    virtual bool isConnectionOriented() const = 0;

};
#endif
