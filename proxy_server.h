#ifndef PROXY_PROXTSERVER_H
#define PROXY_PROXTSERVER_H

#include <map>
#include "client.h"
#include "cache_storage.h"
#include "base_request.h"

class proxy_server {
    const int MAX_VALUE_FOR_PORT = 20000;
    const int MAX_COUNT_OF_PENDING_REQUESTS = 100;
    const char *BIND_IP = "127.0.0.1";
    const int TIME_TO_BE_BLOCKED_IN_POLL = 2000;
    const size_t MAX_CAPACITY_OF_HTTP_REQUEST = 1000;
    const int HTTP_PORT = 80;

    int is_stop;
    int port;
    int socket_fd;
    std::map<int, base_request> requests;

    //char* http_request;

    //void onGetRequestReceived(std::string uri, std::string request);
    //std::string hostname_to_ip(std::string string);
public:
    proxy_server() = delete;
    proxy_server(const proxy_server & proxy_server1) = delete;
    proxy_server & operator= (const proxy_server & p) = delete;
    proxy_server(int port);
    void start();
    void stop();

    virtual ~proxy_server();

};

#endif //PROXY_PROXTSERVER_H
