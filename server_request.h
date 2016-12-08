//
// Created by alexander on 07.12.16.
//

#ifndef PROXY_HANDLEDREQUEST_H
#define PROXY_HANDLEDREQUEST_H

#include <string>
#include <list>
#include "client.h"
#include "base_request.h"

class server_request {
    std::string url;
    int socket_fd;
    std::string request;
    const char* request_char;
    size_t pos;
    size_t size_of_request;

public:
    server_request(int socket_fd, std::string ip, int port) : base_request(socket_fd, ) {
        this -> url = url;
        this -> socket_fd = socket_fd;
        this -> request = request;
        this -> pos = 0;
        this -> request_char = request.c_str();
        this -> size_of_request = request.length();
    }

    int get_socket_fd() {
        return socket_fd;
    }

    const char* get_pointer_to_send() {
        return request_char + pos;
    }

    bool move_pos(size_t step) {
        pos += step;

        return pos >= size_of_request;
    }

    virtual ~server_request() {

    }
};

#endif //PROXY_HANDLEDREQUEST_H
