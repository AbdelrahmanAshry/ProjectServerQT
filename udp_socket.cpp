#include "tcp_socket.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

UDPSocket::UDPSocket(){ }
UDPSocket::~UDPSocket(){ shutdownSocket(); } 

