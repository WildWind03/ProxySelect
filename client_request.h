//
// Created by alexander on 08.12.16.
//

#ifndef PROXY_GET_REQUEST_FROM_CLIENT_H
#define PROXY_GET_REQUEST_FROM_CLIENT_H

#include "base_request.h"
#include "too_much_data_exception.h"
#include "connection_close_exception.h"
#include "exec_request_exception.h"
#include "http_parser.h"
#include "request_not_supported_exception.h"
#include "cache_storage.h"

#include <string>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <bits/poll.h>

class client_request : public base_request {
    const static size_t REQUEST_SIZE = 1000;

    char *request;
    size_t current_pos_in_request;

    bool is_read = true;
    bool is_selectable = false;

    int pos_in_cache_record = 0;
    std::string url;
    cache_storage cache;

    bool is_finished_request(ssize_t count_of_received_bytes) {
        size_t start_pos_for_checking_for_end_of_request = current_pos_in_request - 3 - count_of_received_bytes;
        std::string string(request, start_pos_for_checking_for_end_of_request, current_pos_in_request - start_pos_for_checking_for_end_of_request);

        char end_of_request[] = {'\r', '\n', '\r', '\n'};
        size_t pos_of_end = string.find(end_of_request);

        return pos_of_end != string.length();
    }

public:
    client_request(int socket_fd, std::string ip, int port) : base_request(socket_fd, port, ip) {
        request = (char*) malloc (sizeof(char) * REQUEST_SIZE);
    }

    virtual request_enum exec() override {
        if (is_read) {
            ssize_t count_of_received_bytes = recv(get_socket(), request, REQUEST_SIZE - current_pos_in_request, 0);

            if (-1 == count_of_received_bytes) {
                std::cout << "Error while receiving data" << std::endl;
                throw exec_request_exception("Error while receiving data");
            }

            current_pos_in_request += count_of_received_bytes;

            if (0 == count_of_received_bytes) {
                std::cout << "The connection is closed!" << std::endl;
                throw connection_close_exception(
                        "The connection with " + get_ip() + ":" + std::to_string(get_port()) + "is closed");
            } else {

                if (is_finished_request(count_of_received_bytes)) {
                    std::cout << "The request is received!" << std::endl;

                    http_parser http_parser1(request);

                    int major_v = http_parser1.get_major_version();
                    int minor_v = http_parser1.get_minor_version();

                    url = http_parser1.get_uri();

                    if (major_v != 1 || minor_v != 0) {
                        std::cout << "The version of http protocol is not supported" << std::endl;
                    } else {
                        int request_type = http_parser1.get_request_type();
                        switch (request_type) {
                            case http_parser::GET_REQUEST :
                                return request_enum::READ_FROM_CLIENT_FINISHED;
                            default:
                                std::cout << "The type of request is not supported" << std::endl;
                                throw request_not_supported_exception("Type of request is not GET");
                        }
                    }
                } else {
                    return request_enum::READ;
                }
            }

            return request_enum::READ;
        } else {
            auto data = cache.get_data_by_url(url);
            size_t cur_length = *data.length;

            if (pos_in_cache_record >= cur_length) {
                is_selectable = false;
                return request_enum::NOTHING_TO_WRITE_TO_CLIENT;
            } else {
                ssize_t count_of_sent_chars = send(get_socket(), data.data + pos_in_cache_record, cur_length - pos_in_cache_record, NULL);

                if (-1 == count_of_sent_chars) {
                    throw exec_request_exception("Error while sending data to client");
                }

                pos_in_cache_record += count_of_sent_chars;

                if (*data.isFinished) {
                    if (*data.length == pos_in_cache_record) {
                        return request_enum::WRITE_TO_CLIENT_FINISHED;
                    } else {
                        return request_enum::WRITE;
                    }
                } else {
                    return request_enum::WRITE;
                }
            }
        }
    }

    const char* get_request() {
        return request;
    }

    void change_to_write_mode(cache_storage cache) {
        is_read = false;
        this -> cache = cache;
    }

    void notify_new_data() {
        this -> is_selectable = true;
    }

    bool can_be_selected() {
        return this -> is_selectable;
    }

    virtual short get_socket_select_event() override {
        short polling_event = POLLIN;
        short pollout_event = POLLOUT;

        return is_read ? polling_event : pollout_event;
    }

    virtual ~client_request() {
        free(request);
    }
};
#endif //PROXY_GET_REQUEST_FROM_CLIENT_H
