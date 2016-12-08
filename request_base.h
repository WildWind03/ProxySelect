//
// Created by alexander on 08.12.16.
//

#ifndef PROXY_BASE_REQUEST_H
#define PROXY_BASE_REQUEST_H

#include <string>
#include <exception>
#include "request_enum.h"

class request_base {
    bool is_active = true;
    int socket_fd;
    int port;
    std::string ip;

public:

    request_base(int socket_fd, int port, std::string ip) {
        this -> socket_fd = socket_fd;
        this -> port;
        this -> ip = ip;
    }

    void change_activity(bool new_activity) {
        this -> is_active = new_activity;
    }

    bool is_active_request() {
        return is_active;
    }

    int get_port() {
        return port;
    }

    std::string get_ip() {
        return ip;
    }

    int get_socket() {
        return socket_fd;
    }

    virtual short get_socket_select_event() = 0;

    virtual request_enum exec() = 0;

};

#endif //PROXY_BASE_REQUEST_H
