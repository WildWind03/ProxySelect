//
// Created by alexander on 08.12.16.
//

#ifndef PROXY_EXEC_REQUEST_EXCEPTION_H
#define PROXY_EXEC_REQUEST_EXCEPTION_H

#include <string>
#include <exception>

class exception_read : public std::exception {
    std::string error;

public:
    exception_read(std::string string) : error(string) {

    }

    exception_read & operator= (const exception_read & except) = delete;
    exception_read() = delete;

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override {
            return error.c_str();
    }

    virtual ~exception_read() {

    }
};
#endif //PROXY_EXEC_REQUEST_EXCEPTION_H
