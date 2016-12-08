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
    cache_storage::cached_data *cached_data1;

public:
    request_server(int socket_fd, std::string ip, int port, std::string request, size_t size) : request_base(socket_fd, port, ip) {
        this -> request = request;
        this -> request_size = size;

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
            ssize_t count_of_received_bytes = recv(get_socket(), cached_data1 -> data + cached_data1 -> length,
                                           cache_storage::CACHE_RECORD_MAX_CAPACITY - cached_data1 -> length, 0);

            if (-1 == count_of_received_bytes) {
                std::cout << "Error while receiving data" << std::endl;
                throw exception_read("Error while receiving data");
            }

            cached_data1 -> length += count_of_received_bytes;

            if (0 == count_of_received_bytes) {
                std::cout << "The connection is closed by server!" << std::endl;
                throw exception_connection_closed(
                        "The connection with " + get_ip() + ":" + std::to_string(get_port()) + " is closed");
            }

            if (cached_data1 -> length >= cache_storage::CACHE_RECORD_MAX_CAPACITY) {
                cached_data1 -> is_streaming = true;
                set_selectable(false);
            }

            if (is_finished_request(count_of_received_bytes, cached_data1 -> length, cached_data1 -> data)) {
                cached_data1 -> is_finished = true;
                return request_enum::READ_FROM_SERVER_FINISHED;
            }

        }
    }

    virtual void change_to_read_mode(cache_storage::cached_data *cached_data1) {
        is_write = false;
        this -> cached_data1 = cached_data1;
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
