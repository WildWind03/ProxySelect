//
// Created by alexander on 08.12.16.
//

#ifndef PROXY_HTTP_PARSE_EXCEPTION_H
#define PROXY_HTTP_PARSE_EXCEPTION_H

#include <string>
#include <exception>

class http_parse_exception : public std::exception {
    std::string error;

public:
    http_parse_exception(std::string string) : error(string) {

    }

    http_parse_exception & operator= (const http_parse_exception & except) = delete;
    http_parse_exception() = delete;

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override {
        return error.c_str();
    }

    virtual ~http_parse_exception() {

    }
};
#endif //PROXY_HTTP_PARSE_EXCEPTION_H
