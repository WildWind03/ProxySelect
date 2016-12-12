#ifndef PROXY_PROXTSERVER_H
#define PROXY_PROXTSERVER_H

#include <map>
#include "cached_data.h"
#include "request_base.h"
#include "request_client.h"
#include "request_server.h"

class proxy_server {
    const int MAX_VALUE_FOR_PORT = 20000;
    const int MAX_COUNT_OF_PENDING_REQUESTS = 100;
    const char *BIND_IP = "127.0.0.1";
    const int TIME_TO_BE_BLOCKED_IN_POLL = 2000;
    const int HTTP_PORT = 80;

    int is_stop = false;
    int port;
    int socket_fd;
    std::map<int, request_base*> requests;
    std::multimap<std::string, cached_data*> storage;

    void onRequestSatisfied(request_client* request_client1);
    void onGetRequestReceived(request_client* request_client1);
    void onRequestPassedToServer(request_server *request_server1);
    void onResponseReceivedFromServer(request_server *request_server1);

    std::string hostname_to_ip(std::string string);
    void delete_record_from_cache(cached_data *cached_data1);
    void delete_all_clients_connected_to_cache_data(cached_data *cached_data1);
    void delete_request(request_base* base_request1);
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
