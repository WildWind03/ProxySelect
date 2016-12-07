//
// Created by alexander on 07.12.16.
//

#ifndef PROXY_HTTP_PARSER_H
#define PROXY_HTTP_PARSER_H

#include <string>
#include <sstream>
#include <iostream>

class http_parser {
    int request_type;
    std::string uri;
    int major_version;
    int minor_version;
    std::string host;

public:
    const static int GET_REQUEST = 0;
    const static int OTHER_REQUEST = 1;

    http_parser(const char* request) {
        std::stringstream stringstream(request);

        std::string request_type_str;
        stringstream >> request_type_str;
        if (request_type_str == "GET") {
            request_type = GET_REQUEST;
        } else {
            request_type = OTHER_REQUEST;
        }

        stringstream >> uri;

        size_t start = uri.find('/') + 2;
        size_t end = uri.find('/', start);
        host = uri.substr(start, end - start);

        std::string protocol_version;
        stringstream >> protocol_version;

        size_t index_of_slash = protocol_version.find('/') + 1;
        size_t index_of_end_of_line = protocol_version.find('\n');
        std::string protocol_version_short = protocol_version.substr(index_of_slash, index_of_end_of_line);
        size_t index_of_dot = protocol_version_short.find('.');

        std::string major_version = protocol_version_short.substr(0, index_of_dot);
        std::string minor_version = protocol_version_short.substr(index_of_dot + 1);

        this->major_version = std::stoi(major_version);
        this->minor_version = std::stoi(minor_version);

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
