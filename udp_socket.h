#ifndef UDP_SOCKET_H
#define UDP_SOCKET_H
#include "Socket.h"
#include <boost/asio.hpp>
#include <string>
#include <memory>

class UDPSocket : public Socket {
private:
    boost::asio::io_context io;
    std::unique_ptr<boost::asio::ip::udp::socket> socket;
    boost::asio::ip::udp::endpoint remote;
    bool remoteSet = false;

public:
    UDPSocket();
    ~UDPSocket() ;
   // UDPSocket(boost::asio::ip::udp::socket &fola);
//    bool isConnectionOriented() ;
    bool isConnectionOriented() const override { return false; }  
    void waitForConnect(int port) ;
    void connectToServer(const std::string& host, int port) ;
    unsigned int send(const std::string& msg) ;
    unsigned int receive(std::string& out) override;
    //unsigned int receive(std::string&, boost::asio::ip::udp::endpoint* sender = nullptr);
    void shutdown() ;
};


#endif