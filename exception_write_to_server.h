//
// Created by alexander on 12.12.16.
//

#ifndef PROXY_EXCEPTION_WRITE_TO_SERVER_H
#define PROXY_EXCEPTION_WRITE_TO_SERVER_H

#include <exception>
#include <string>

class exception_write_to_server : public std::exception {
    std::string error;

public:
    exception_write_to_server(std::string string) : error(string) {

    }

    exception_write_to_server & operator= (const exception_write_to_server & except) = delete;
    exception_write_to_server() = delete;

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override {
            return error.c_str();
    }

    virtual ~exception_write_to_server() {

    }
};
#endif //PROXY_EXCEPTION_WRITE_TO_SERVER_H
