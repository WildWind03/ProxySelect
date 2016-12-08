//
// Created by alexander on 07.12.16.
//

#ifndef PROXY_TOO_MANY_DATA_EXCEPTION_H
#define PROXY_TOO_MANY_DATA_EXCEPTION_H

#include <exception>
#include <string>

class exception_too_much_data : public std::exception {
    std::string error;
public:
    exception_too_much_data(std::string string) : error(string) {

    }

    exception_too_much_data & operator= (const exception_too_much_data & except) = delete;
    exception_too_much_data() = delete;

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override {
        return error.c_str();
    }

    virtual ~exception_too_much_data() {

    }
};

#endif //PROXY_TOO_MANY_DATA_EXCEPTION_H
