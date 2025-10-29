#include "tcp_socket.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

UDPSocket::UDPSocket(){ }
UDPSocket::~UDPSocket(){ shutdownSocket(); } 

void UDPSocket::waitForConnect(int port){
    listenFd = socket(AF_INET, SOCK_DGRAM, 0);
    int opt=1; 
    setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));///reusable address
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);//Bind to Port 
    addr.sin_addr.s_addr=INADDR_ANY;
    bind(listenFd,(struct sockaddr*)&addr,sizeof(addr));
    std::cout<<"UDP Socket created \n";

    listen(listenFd,5);
    std::cout<<"UDP listening "<<port<<"\n";

    struct sockaddr_in cli{};
    socklen_t len=sizeof(cli);
    connFd = accept(listenFd,(sockaddr*)&cli,&len);
    std::cout<<"UDP accepted\n";
}
void UDPSocket::connect(const std::string &host,int port){
    connFd =socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in serv{};
    serv.sin_family=AF_INET;
    serv.sin_port=htons(port);
    inet_pton(AF_INET, host.c_str(), &serv.sin_addr);
    connect(connFd,(sockaddr*)&serv,sizeof(serv));//more than 0 means success
    std::cout<<"UDP connected to "<<host<<":"<<port<<"\n";
}
void UDPSocket::shutdown(){
    if(connFd>0){  
        close(connFd); 
        connFd=-1; 
    }
    if(listenFd>0){ 
        close(listenFd); 
        listenFd=-1; 
    }
}
unsigned int UDPSocket::send(const std::string &msg,const sockaddr_in*){
    return send(connFd, msg.data(), msg.size(), 0);
}
unsigned int UDPSocket::receive(std::string &out, sockaddr_in*){
    char buf[1024];
//    unsigned int n =recv(connFd, buf, sizeof(buf)-1, 0);
    unsigned int n =read(connFd, buf, sizeof(buf)-1);
    if(n>0){ buf[n]=0; out.assign(buf,n); }
    return n;
}
