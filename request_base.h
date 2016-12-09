//
// Created by alexander on 08.12.16.
//

#ifndef PROXY_BASE_REQUEST_H
#define PROXY_BASE_REQUEST_H

#include <string>
#include <exception>
#include "request_enum.h"

class request_base : public observer {
    bool selectable = true;
    int socket_fd;
    int port;
    std::string ip;

protected:
    bool is_finished_request(size_t count_of_received_bytes, size_t current_pos_in_request, char* request) {
        size_t start_pos_for_checking_for_end_of_request;
        size_t length;

        if (current_pos_in_request >= 3 ) {
            start_pos_for_checking_for_end_of_request = current_pos_in_request - 3;
            length = count_of_received_bytes + 3;
        } else {
            start_pos_for_checking_for_end_of_request = 0;
            length = count_of_received_bytes;
        }

        std::string string(request + start_pos_for_checking_for_end_of_request, length);

        char end_of_request[] = {'\r', '\n', '\r', '\n'};
        size_t pos_of_end = string.find(end_of_request);

        return pos_of_end != string.length();
    }

public:
    request_base(int socket_fd, int port, std::string ip) {
        this -> socket_fd = socket_fd;
        this -> port;
        this -> ip = ip;
    }

    void set_selectable(bool selectable) {
        this -> selectable = selectable;
    }

    bool is_selectable() {
        return selectable;
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
