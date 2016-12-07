//
// Created by alexander on 01.12.16.
//

#ifndef PROXY_INETSOCKETADDRESS_H
#define PROXY_INETSOCKETADDRESS_H

#include <string>
#include <cstring>
#include "too_much_data_exception.h"


class client {
    const static size_t REQUEST_SIZE = 1000;

    int port;
    std::string ip;
    std::string request;
    size_t current_pos_in_request;

public:
    client(int port, std::string ip) {
        this->ip = ip;
        this->port = port;
        current_pos_in_request = 0;
    }

    int getPort() {
        return port;
    }

    std::string getIP() {
        return ip;
    }

    void add_new_data(char *string, size_t size) {
        if (size > REQUEST_SIZE - current_pos_in_request) {
            throw too_much_data_exception("Too much data exception");
        }

        request.append(string);
        current_pos_in_request += size;
    }

    size_t get_max_data_to_read() {
        return REQUEST_SIZE - current_pos_in_request;
    }

    std::string get_request() {
        return request.c_str();
    }

    size_t get_request_length() {
        return current_pos_in_request;
    }

    bool is_correct_request() {
        char end_of_request[] = {'\r', '\n', '\r', '\n'};
        size_t pos_of_end = request.find(request);

        return pos_of_end != current_pos_in_request;
    }

    virtual ~client() {

    }

};


#endif //PROXY_INETSOCKETADDRESS_H
