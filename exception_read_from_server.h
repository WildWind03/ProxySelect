//
// Created by alexander on 12.12.16.
//

#ifndef PROXY_EXCEPTION_READ_FROM_SERVER_H
#define PROXY_EXCEPTION_READ_FROM_SERVER_H

#include <exception>
#include <string>

class exception_read_from_server : public std::exception {
    std::string error;

public:
    exception_read_from_server(std::string string) : error(string) {

    }

    exception_read_from_server & operator= (const exception_read_from_server & except) = delete;
    exception_read_from_server() = delete;

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override {
            return error.c_str();
    }

    virtual ~exception_read_from_server() {

    }
};
#endif //PROXY_EXCEPTION_READ_FROM_SERVER_H
