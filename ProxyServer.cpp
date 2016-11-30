#include <sys/socket.h>
#include "ProxyServer.h"

ProxyServer::ProxyServer(int port) {
    this -> port = port;
    socketFd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr sockaddr1;
    sockaddr1.sa_data;
    sockaddr1.sa_family;
    bind(socketFd, sockaddr1, )
}

ProxyServer::~ProxyServer() {

}

void ProxyServer::start() {

}



