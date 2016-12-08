//
// Created by alexander on 08.12.16.
//

#ifndef PROXY_CONNECTION_CLOSE_EXCEPTION_H
#define PROXY_CONNECTION_CLOSE_EXCEPTION_H

#include <exception>
#include <string>

class connection_close_exception : public std::exception {
    std::string error;

public:
    connection_close_exception(std::string string) : error(string) {

    }

    connection_close_exception & operator= (const connection_close_exception & except) = delete;
    connection_close_exception() = delete;

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override {
            return error.c_str();
    }

    virtual ~connection_close_exception() {

    }
};
#endif //PROXY_CONNECTION_CLOSE_EXCEPTION_H
