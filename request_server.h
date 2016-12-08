//
// Created by alexander on 07.12.16.
//

#ifndef PROXY_HANDLEDREQUEST_H
#define PROXY_HANDLEDREQUEST_H

#include <string>
#include <list>
#include <bits/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include "request_base.h"
#include "exception_can_not_connect.h"
#include "exception_write.h"

class request_server : public request_base {
    std::string request;
    size_t request_size;

    bool is_write = true;
    size_t pos_in_sending_data = 0;

public:
    request_server(int socket_fd, std::string ip, int port, std::string request, size_t size) : request_base(socket_fd, port, ip) {
        this -> request = request;
        this -> request_size = request_size;

        struct sockaddr_in sockaddr_in1;
        sockaddr_in1.sin_family = AF_INET;
        sockaddr_in1.sin_port = htons(port);
        sockaddr_in1.sin_addr.s_addr = inet_addr(ip.c_str());

        int connect_result = connect(socket_fd, (struct sockaddr *) &sockaddr_in1, sizeof(sockaddr_in1));

        if (-1 == connect_result) {
            throw exception_can_not_connect("Can not connect to server");
        }
    }

    virtual request_enum exec() override {
        if (is_write) {
            ssize_t count_of_send_data = send(get_socket(), request.c_str() + pos_in_sending_data,
                                              request_size - pos_in_sending_data, 0);

            if (-1 == count_of_send_data) {
                throw exception_write("Error while writing");
            }

            pos_in_sending_data += count_of_send_data;

            if (pos_in_sending_data == request_size) {
                return request_enum::WRITE_TO_SERVER_FINISHED;
            } else {
                return request_enum::WRITE;
            }
        } else {

        }

    }

    virtual void change_to_read_mode() {
        is_write = false;
    }

    virtual short get_socket_select_event() override {
        short write_event = POLLOUT;
        short read_event = POLLIN;
        return is_write ? write_event : read_event;
    }

    virtual ~request_server() {

    }
};

#endif //PROXY_HANDLEDREQUEST_H
