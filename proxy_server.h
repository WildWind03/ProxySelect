#ifndef PROXY_PROXTSERVER_H
#define PROXY_PROXTSERVER_H

#include <map>
#include "inet_socket_address.h"
#include "http-parser-master/http_parser.h"

class proxy_server {
    const int MAX_VALUE_FOR_PORT = 20000;
    const int MAX_COUNT_OF_PENDING_REQUESTS = 100;
    const char *BIND_IP = "127.0.0.1";
    const int TIME_TO_BE_BLOCKED_IN_POLL = 2000;
    const size_t MAX_CAPACITY_OF_HTTP_REQUEST = 1000;

    int is_stop;
    int port;
    int socketFd;
    std::map<int, inet_socket_address> sockets_we_wait_for_request;
    std::map<inet_socket_address, int> sockets_we_wait_for_response;
    std::map<inet_socket_address, int> sockets_we_want_to_request_to;

    char* http_request;
    http_parser *parser;
    http_parser_settings *http_parser_settings1;

public:
    proxy_server() = delete;
    proxy_server(const proxy_server & proxy_server1) = delete;
    proxy_server & operator= (const proxy_server & p) = delete;
    proxy_server(int port);
    void start();
    void stop();

    int my_header_field_callback(http_parser* parser1, const char * header, size_t size);
    static int my_header_field_callback_static(http_parser* parser1, const char * header, size_t size);

    virtual ~proxy_server();
};

#endif //PROXY_PROXTSERVER_H
