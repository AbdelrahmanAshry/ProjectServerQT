#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H
#include "Socket.h"
#include <boost/asio.hpp>
#include <string>
#include <memory>

class TCPSocket : public Socket {
private:
    boost::asio::io_context io;
    std::unique_ptr<boost::asio::ip::tcp::socket> socket;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor;

public:
    TCPSocket();
    ~TCPSocket() ;
    bool isConnectionOriented() const override { return true; } 

   // bool isConnectionOriented() ;
    void waitForConnect(int port) ;
    void connectToServer(const std::string& host, int port) ;
    unsigned int send(const std::string& msg) ;
    unsigned int receive(std::string& out) ;
    void shutdown() ;
};
#endif