//
// Created by alexander on 08.12.16.
//

#ifndef PROXY_EXEC_REQUEST_EXCEPTION_H
#define PROXY_EXEC_REQUEST_EXCEPTION_H

#include <string>
#include <exception>

class exec_request_exception : public std::exception {
    std::string error;

public:
    exec_request_exception(std::string string) : error(string) {

    }

    exec_request_exception & operator= (const exec_request_exception & except) = delete;
    exec_request_exception() = delete;

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override {
            return error.c_str();
    }

    virtual ~exec_request_exception() {

    }
};
#endif //PROXY_EXEC_REQUEST_EXCEPTION_H
