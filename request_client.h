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
#include "logger.h"
#include "url_util.h"

#include <string>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <sys/poll.h>
#include <algorithm>

class request_client : public request_base {
    const static size_t REQUEST_SIZE = 4096;

    char *request;
    size_t current_pos_in_request = 0;

    bool is_read = true;

    std::string url;
    cached_data *cache;
    std::string host;

    logger *logger1;

public:
    request_client(int socket_fd, std::string ip, int port) : request_base(socket_fd, port, ip) {
        logger1 = new logger(ip + ":" + std::to_string(port));
        request = (char*) malloc (sizeof(char) * REQUEST_SIZE);
    }

    virtual void update(event_type event_type1) override {
        if (event_type1 == event_type::ENABLE_READ) {
            set_selectable(true);
        }

        if (event_type1 == event_type::DISABLE_READ) {
            set_selectable(false);
        }
    }

    virtual request_enum exec() override {
        if (is_read) {
            ssize_t count_of_received_bytes = recv(get_socket(), request, REQUEST_SIZE - current_pos_in_request, 0);

            if (-1 == count_of_received_bytes) {
                logger1 -> log("Error while receiving data");
                throw exception_read("Error while receiving data");
            }

            current_pos_in_request += count_of_received_bytes;

            if (0 == count_of_received_bytes) {
                logger1 -> log("The connection is closed");
                throw exception_connection_closed(
                        "The connection with " + get_ip() + ":" + std::to_string(get_port()) + " is closed");
            }
            if (is_finished_request(count_of_received_bytes, current_pos_in_request, request)) {
                std::string handled_request = request;
                free(request);


                handled_request = url_util::change_type_of_connection_to_close(handled_request);

                request = (char*) malloc(sizeof(char) * handled_request.length());

                for (int k = 0; k < handled_request.length(); ++k) {
                    request[k] = handled_request.c_str()[k];
                }

                http_request_parser http_parser1(request);

                int major_v = http_parser1.get_major_version();
                int minor_v = http_parser1.get_minor_version();

                url = http_parser1.get_uri();
                host = http_parser1.get_host();

                if (major_v != 1 || minor_v != 0) {
                    std::cout << "The version of http protocol is not supported" << std::endl;
                    throw exception_not_supported_request("Not supported version of protocol");
                } else {
                    int request_type = http_parser1.get_request_type();
                    switch (request_type) {
                        case http_request_parser::GET_REQUEST : {
                            logger1->log("The GET request is got from client!");
                            delete logger1;

                            logger1 = new logger("client", "/home/alexander/ClionProjects/Proxy/log/" + url_util::get_logger_filename_by_url(url));

                            return request_enum::READ_FROM_CLIENT_FINISHED;
                        }
                        default:
                            logger1 -> log ("Type of request is not GET");
                            throw exception_not_supported_request("Type of request is not GET");
                    }
                }
            } else {
                return request_enum::READ;
            }
        } else {

            auto data = cache;
            ssize_t count_of_sent_chars = send(get_socket(), data -> get_data_to_read(get_socket()), data -> get_count_of_bytes_that_can_be_read(get_socket()), 0);

            logger1 -> log (std::to_string(count_of_sent_chars) + " was sent to the browser");
            bool result_od_send = data -> update_because_data_was_read(get_socket(), count_of_sent_chars);

            if(result_od_send) {
                logger1 -> log ("The request was FINISHED and the data was sent to browser!");
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

    void log(std::string string) {
        logger1 -> log(string);
    }

    cached_data* get_cached_data() {
        return cache;
    }

    virtual ~request_client() {
        free(request);
        delete(logger1);
    }
};
#endif //PROXY_GET_REQUEST_FROM_CLIENT_H
