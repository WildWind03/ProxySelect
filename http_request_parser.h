//
// Created by alexander on 07.12.16.
//

#ifndef PROXY_HTTP_PARSER_H
#define PROXY_HTTP_PARSER_H

#include <string>
#include <sstream>
#include <iostream>
#include "exception_invalid_http_data.h"
#include "request_client.h"

class http_request_parser {
    int request_type;
    std::string uri;
    int major_version;
    int minor_version;
    std::string host;

    std::string request_type_str;

public:
    const static int GET_REQUEST = 0;
    const static int OTHER_REQUEST = 1;

    http_request_parser(const char* request) {
        std::stringstream stringstream(request);

        stringstream >> request_type_str;
        if (request_type_str == "GET") {
            request_type = GET_REQUEST;
        } else {
            request_type = OTHER_REQUEST;
        }

        stringstream >> uri;

        size_t start = uri.find('/') + 2;
        size_t end = uri.find('/', start);

        if (start >= end) {
            throw exception_invalid_http_data("Invalid hostname");
        }

        host = uri.substr(start, end - start);

        std::string protocol_version;
        stringstream >> protocol_version;

        size_t index_of_slash = protocol_version.find('/') + 1;
        size_t index_of_end_of_line = protocol_version.find('\n');
        if (index_of_end_of_line <= index_of_slash) {
            throw exception_invalid_http_data("Invalid protocol version");
        }

        std::string protocol_version_short = protocol_version.substr(index_of_slash, index_of_end_of_line);
        size_t index_of_dot = protocol_version_short.find('.');

        std::string major_version;
        std::string minor_version;

        try {
            major_version = protocol_version_short.substr(0, index_of_dot);
            minor_version = protocol_version_short.substr(index_of_dot + 1);

            this->major_version = std::stoi(major_version);
            this->minor_version = std::stoi(minor_version);

        } catch (std::exception & e) {
            throw exception_invalid_http_data("Invalid request");
        }

    }

    std::string get_request_type_str() {
        return request_type_str;
    }

    int get_request_type() {
        return request_type;
    }

    std::string get_uri() {
        return uri;
    }

    std::string get_host() {
        return host;
    }

    int get_major_version() {
        return major_version;
    }

    int get_minor_version() {
        return minor_version;
    }
};

#endif //PROXY_HTTP_PARSER_H
