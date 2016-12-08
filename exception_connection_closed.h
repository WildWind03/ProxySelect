//
// Created by alexander on 08.12.16.
//

#ifndef PROXY_CONNECTION_CLOSE_EXCEPTION_H
#define PROXY_CONNECTION_CLOSE_EXCEPTION_H

#include <exception>
#include <string>

class exception_connection_closed : public std::exception {
    std::string error;

public:
    exception_connection_closed(std::string string) : error(string) {

    }

    exception_connection_closed & operator= (const exception_connection_closed & except) = delete;
    exception_connection_closed() = delete;

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override {
            return error.c_str();
    }

    virtual ~exception_connection_closed() {

    }
};
#endif //PROXY_CONNECTION_CLOSE_EXCEPTION_H
