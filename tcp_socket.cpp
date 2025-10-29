#include "tcp_socket.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

TCPSocket::TCPSocket(){ }
TCPSocket::~TCPSocket(){ shutdownSocket(); } 

void TCPSocket::waitForConnect(int port){
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    int opt=1; 
    setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));//different ports better
    struct sockaddr_in addr={0};
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);//Bind to Port 
    addr.sin_addr.s_addr=INADDR_ANY;

    bind(listenFd,(struct sockaddr*)&addr,sizeof(addr));
    std::cout<<"TCP Socket created \n";

    listen(listenFd,3);
    std::cout<<"TCP listening "<<port<<"\n";

    struct sockaddr_in cli={0};
    socklen_t len=sizeof(cli);
    connFd = accept(listenFd,(sockaddr*)&cli,&len);
    std::cout<<"TCP accepted\n";
}
void TCPSocket::connect(const std::string &host,int port){
    connFd =socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv{};
    serv.sin_family=AF_INET;
    serv.sin_port=htons(port);
    inet_pton(AF_INET, host.c_str(), &serv.sin_addr);
    connect(connFd,(sockaddr*)&serv,sizeof(serv));//more than 0 means success
    std::cout<<"TCP connected to "<<host<<":"<<port<<"\n";
}
void TCPSocket::shutdown(){
    if(connFd>0){  
        close(connFd); 
        connFd=-1; 
    }
    if(listenFd>0){ 
        close(listenFd); 
        listenFd=-1; 
    }
}
unsigned int TCPSocket::send(const std::string &msg,const sockaddr_in*){
    return send(connFd, msg.data(), msg.size(), 0);
}
unsigned int TCPSocket::receive(std::string &out, sockaddr_in*){
    char buf[1024];
//    unsigned int n =recv(connFd, buf, sizeof(buf)-1, 0);
    unsigned int n =read(connFd, buf, sizeof(buf)-1);
    if(n>0){ buf[n]=0; out.assign(buf,n); }
    return n;
}
