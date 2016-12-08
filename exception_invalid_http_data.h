//
// Created by alexander on 08.12.16.
//

#ifndef PROXY_HTTP_PARSE_EXCEPTION_H
#define PROXY_HTTP_PARSE_EXCEPTION_H

#include <string>
#include <exception>

class exception_invalid_http_data : public std::exception {
    std::string error;

public:
    exception_invalid_http_data(std::string string) : error(string) {

    }

    exception_invalid_http_data & operator= (const exception_invalid_http_data & except) = delete;
    exception_invalid_http_data() = delete;

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override {
        return error.c_str();
    }

    virtual ~exception_invalid_http_data() {

    }
};
#endif //PROXY_HTTP_PARSE_EXCEPTION_H
