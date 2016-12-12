//
// Created by alexander on 08.12.16.
//

#ifndef PROXY_EXCEPTION_WRITE_H
#define PROXY_EXCEPTION_WRITE_H

#include <string>
#include <exception>

class exception_write_to_browser : public std::exception {
    std::string error;

public:
    exception_write_to_browser(std::string string) : error(string) {

    }

    exception_write_to_browser & operator= (const exception_write_to_browser & except) = delete;
    exception_write_to_browser() = delete;

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override {
        return error.c_str();
    }

    virtual ~exception_write_to_browser() {

    }
};

#endif //PROXY_EXCEPTION_WRITE_H
