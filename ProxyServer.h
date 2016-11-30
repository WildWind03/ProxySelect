#ifndef PROXY_PROXTSERVER_H
#define PROXY_PROXTSERVER_H

class ProxyServer {
    int port;
    int socketFd;

public:
    ProxyServer() = delete;
    ProxyServer(const ProxyServer & proxyServer) = delete;
    ProxyServer & operator= (const ProxyServer & p) = delete;
    ProxyServer(int port);
    void start();

    virtual ~ProxyServer();
};

#endif //PROXY_PROXTSERVER_H
