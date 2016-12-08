//
// Created by alexander on 08.12.16.
//

#ifndef PROXY_EXCEPTION_CAN_NOT_CONNECT_H
#define PROXY_EXCEPTION_CAN_NOT_CONNECT_H
#include <exception>
#include <string>

class exception_can_not_connect : public std::exception {
    std::string error;

public:
    exception_can_not_connect(std::string string) : error(string) {

    }

    exception_can_not_connect & operator= (const exception_can_not_connect & except) = delete;
    exception_can_not_connect() = delete;

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override {
        return error.c_str();
    }

    virtual ~exception_can_not_connect() {

    }
};
#endif //PROXY_EXCEPTION_CAN_NOT_CONNECT_H
