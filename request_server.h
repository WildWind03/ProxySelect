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
    cached_data *cached_data1;

public:
    request_server(int socket_fd, std::string ip, int port, std::string request, size_t size, cached_data* cached_data2) : request_base(socket_fd, port, ip) {
        this -> request = request;
        this -> request_size = size;
        this -> cached_data1 = cached_data2;

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
            std::cout << cached_data1 -> get_max_data_can_be_written() << std::endl;

            std::cout << cached_data1 -> get_data_to_write() << std::endl;

            ssize_t count_of_received_bytes = recv(get_socket(), cached_data1 -> get_data_to_write(),
                                           cached_data1 -> get_max_data_can_be_written(), 0);

            if (-1 == count_of_received_bytes) {
                std::cout << "Error while receiving data" << std::endl;
                throw exception_read("Error while receiving data");
            }

            if (0 == count_of_received_bytes) {
                cached_data1 -> mark_finished();
                return request_enum::READ_FROM_SERVER_FINISHED;
            }

            cached_data1 -> update_because_data_was_add(count_of_received_bytes);

            return request_enum::READ;
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

    virtual void update(event_type event_type1) override {
        if (event_type1 == event_type::ENABLE_WRITE) {
            set_selectable(true);
        }

        if (event_type1 == event_type::DISABLE_WRITE) {
            set_selectable(false);
        }
    }

    virtual ~request_server() {

    }
};

#endif //PROXY_HANDLEDREQUEST_H
