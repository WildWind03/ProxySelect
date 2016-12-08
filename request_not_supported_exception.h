//
// Created by alexander on 08.12.16.
//

#ifndef PROXY_REQUEST_NOT_SUPPORTED_EXCEPTION_H
#define PROXY_REQUEST_NOT_SUPPORTED_EXCEPTION_H

#include <string>
#include <exception>

class request_not_supported_exception : public std::exception {
    std::string error;

public:
    request_not_supported_exception(std::string string) : error(string) {

    }

    request_not_supported_exception & operator= (const request_not_supported_exception & except) = delete;
    request_not_supported_exception() = delete;

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override {
            return error.c_str();
    }

    virtual ~request_not_supported_exception() {

    }
};
#endif //PROXY_REQUEST_NOT_SUPPORTED_EXCEPTION_H
