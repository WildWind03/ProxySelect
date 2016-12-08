//
// Created by alexander on 08.12.16.
//

#ifndef PROXY_REQUEST_NOT_SUPPORTED_EXCEPTION_H
#define PROXY_REQUEST_NOT_SUPPORTED_EXCEPTION_H

#include <string>
#include <exception>

class exception_not_supported_request : public std::exception {
    std::string error;

public:
    exception_not_supported_request(std::string string) : error(string) {

    }

    exception_not_supported_request & operator= (const exception_not_supported_request & except) = delete;
    exception_not_supported_request() = delete;

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override {
            return error.c_str();
    }

    virtual ~exception_not_supported_request() {

    }
};
#endif //PROXY_REQUEST_NOT_SUPPORTED_EXCEPTION_H
