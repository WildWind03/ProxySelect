#ifndef PROXY_PROXTSERVER_H
#define PROXY_PROXTSERVER_H

#include <map>
#include "InetSocketAddress.h"

class ProxyServer {
    const int MAX_VALUE_FOR_PORT = 20000;
    const int MAX_COUNT_OF_PENDING_REQUESTS = 100;
    const char *BIND_IP = "127.0.0.1";
    const int TIME_TO_BE_BLOCKED_IN_POLL = 2000;

    int port;
    int socketFd;
    std::map<InetSocketAddress, int> sockets;

public:
    ProxyServer() = delete;
    ProxyServer(const ProxyServer & proxyServer) = delete;
    ProxyServer & operator= (const ProxyServer & p) = delete;
    ProxyServer(int port);
    void start();

    virtual ~ProxyServer();
};

#endif //PROXY_PROXTSERVER_H
