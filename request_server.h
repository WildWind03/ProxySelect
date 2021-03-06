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
#include "exception_write_to_server.h"
#include "exception_read_from_server.h"

class request_server : public request_base {
    std::string request;
    size_t request_size;

    std::string url;

    bool is_write = true;
    size_t pos_in_sending_data = 0;
    cached_data *cached_data1;
    logger *logger1;

    bool is_connected;

public:
    request_server(int socket_fd, addrinfo *addrinfo1, int port, std::string request, size_t size, cached_data* cached_data2, std::string url) : request_base(socket_fd, port, "127.0.0.1") {
        is_connected = false;

        this -> url = url;
        logger1 = new logger("server", "/home/alexander/ClionProjects/Proxy/log/" + url_util::get_logger_filename_by_url(url));

        logger1->log(request);

        this -> request = request;
        this -> request_size = size;
        this -> cached_data1 = cached_data2;

        bool is_socket_block_mode = url_util::set_socket_blocking_enabled(socket_fd, false);

        if (!is_socket_block_mode) {
            throw exception_can_not_connect("Can't turn a socket on non blocking mode");
        }

        log("Start connect");
        int connect_result = connect(socket_fd, addrinfo1 -> ai_addr, addrinfo1 -> ai_addrlen);
        free(addrinfo1);

        if (0 == connect_result) {
            is_connected = true;
            log("Connect finish");
        }
    }

    virtual request_enum exec() override {
        if (!is_connected) {
            int result;
            socklen_t result_len = sizeof(result);
            if (getsockopt(get_socket(), SOL_SOCKET, SO_ERROR, &result, &result_len) < 0) {
                throw exception_can_not_connect("Can not connect to server " + get_url());
            }

            if (result != 0) {
                throw exception_can_not_connect("Can not connect to server" + get_url());
            }

            is_connected = true;
            log("Connect finish");
        }

        if (is_write) {
            ssize_t count_of_send_data = send(get_socket(), request.c_str() + pos_in_sending_data,
                                              request_size - pos_in_sending_data, 0);

            logger1 -> log(count_of_send_data + " was sent to the server");

            if (-1 == count_of_send_data) {
                logger1 -> log("Error while writng");
                throw exception_write_to_server("Error while writing");
            }

            pos_in_sending_data += count_of_send_data;

            if (pos_in_sending_data == request_size) {
                logger1 -> log ("The request was fully sent to the server!");
                return request_enum::WRITE_TO_SERVER_FINISHED;
            } else {
                return request_enum::WRITE;
            }
        } else {
            ssize_t count_of_received_bytes = recv(get_socket(), cached_data1 -> get_data_to_write(),
                                           cached_data1 -> get_max_data_can_be_written(), 0);

            logger1 -> log(std::to_string(count_of_received_bytes) + " bytes was read");

            if (-1 == count_of_received_bytes) {
                logger1 -> log ("Error while receiving data");
                throw exception_read_from_server("Error while receiving data");
            }

            if (0 == count_of_received_bytes) {
                cached_data1 -> mark_finished();
                logger1 -> log ("The response is received from the server");

                return request_enum::READ_FROM_SERVER_FINISHED;
            }

            bool is_streaming_start = cached_data1 -> is_streaming;
            cached_data1 -> update_because_data_was_add(count_of_received_bytes);
            bool is_streaming_end = cached_data1 -> is_streaming;

            if (!is_streaming_start && is_streaming_end) {
                logger1 -> log ("Stream mode is enabled!");
            }

            return request_enum::READ;
        }
    }

    virtual void change_to_read_mode() {
        is_write = false;
    }

    virtual short get_socket_select_event() override {
        if (!is_connected) {
            return POLLOUT;
        }

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

    std::string get_url() {
        return url;
    }

    void log(std::string string) {
        logger1 -> log(string);
    }

    cached_data* get_cached_data() {
        return cached_data1;
    }

    virtual ~request_server() {
        delete(logger1);
    }
};

#endif //PROXY_HANDLEDREQUEST_H
