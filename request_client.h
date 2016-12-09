//
// Created by alexander on 08.12.16.
//

#ifndef PROXY_GET_REQUEST_FROM_CLIENT_H
#define PROXY_GET_REQUEST_FROM_CLIENT_H

#include "request_base.h"
#include "exception_too_much_data.h"
#include "exception_connection_closed.h"
#include "exception_read.h"
#include "http_request_parser.h"
#include "exception_not_supported_request.h"
#include "cached_data.h"

#include <string>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <sys/poll.h>

class request_client : public request_base {
    const static size_t REQUEST_SIZE = 4096;

    char *request;
    size_t current_pos_in_request = 0;

    bool is_read = true;

    size_t pos_in_cache_record = 0;
    std::string url;
    cached_data *cache;
    std::string host;

public:
    request_client(int socket_fd, std::string ip, int port) : request_base(socket_fd, port, ip) {
        request = (char*) malloc (sizeof(char) * REQUEST_SIZE);
    }

    virtual void update(event_type event_type1) override {
        if (event_type1 == event_type::ENABLE_READ) {

        }

        if (event_type1 == event_type::DISABLE_READ) {

        }
    }

    virtual request_enum exec() override {
        if (is_read) {
            ssize_t count_of_received_bytes = recv(get_socket(), request, REQUEST_SIZE - current_pos_in_request, 0);

            if (-1 == count_of_received_bytes) {
                std::cout << "Error while receiving data" << std::endl;
                throw exception_read("Error while receiving data");
            }

            current_pos_in_request += count_of_received_bytes;

            if (0 == count_of_received_bytes) {
                std::cout << "The connection is closed!" << std::endl;
                throw exception_connection_closed(
                        "The connection with " + get_ip() + ":" + std::to_string(get_port()) + " is closed");
            }
            if (is_finished_request(count_of_received_bytes, current_pos_in_request, request)) {
                //std::cout << "The request is received!" << std::endl;

                http_request_parser http_parser1(request);

                int major_v = http_parser1.get_major_version();
                int minor_v = http_parser1.get_minor_version();

                url = http_parser1.get_uri();
                host = http_parser1.get_host();

                if (major_v != 1 || minor_v != 0) {
                    std::cout << "The version of http protocol is not supported" << std::endl;
                } else {
                    int request_type = http_parser1.get_request_type();
                    switch (request_type) {
                        case http_request_parser::GET_REQUEST :
                            return request_enum::READ_FROM_CLIENT_FINISHED;
                        default:
                            //std::cout << "The type of request is not supported" << std::endl;
                            throw exception_not_supported_request("Type of request is not GET");
                    }
                }
            } else {
                return request_enum::READ;
            }

            return request_enum::READ;
        } else {
            auto data = cache;
            ssize_t count_of_sent_chars = send(get_socket(), data -> get_data_to_read(get_socket()), data -> get_count_of_bytes_that_can_be_read(get_socket()), 0);

            bool result_od_send = data -> update_because_data_was_read(get_socket(), count_of_sent_chars);

            if(result_od_send) {
                return request_enum::WRITE_TO_CLIENT_FINISHED;
            } else {
                return request_enum::WRITE;
            }
        }
    }

    const char* get_request() {
        return request;
    }

    size_t get_size_of_request() {
        return current_pos_in_request;
    }

    void change_to_write_mode(cached_data *cache) {
        is_read = false;
        this -> cache = cache;
    }


    std::string get_url() {
        return url;
    }

    std::string get_host() {
        return host;
    }

    virtual short get_socket_select_event() override {
        short polling_event = POLLIN;
        short pollout_event = POLLOUT;

        return is_read ? polling_event : pollout_event;
    }

    size_t get_pos_in_cache_record() {
        return pos_in_cache_record;
    }

    virtual ~request_client() {
        free(request);
    }
};
#endif //PROXY_GET_REQUEST_FROM_CLIENT_H
