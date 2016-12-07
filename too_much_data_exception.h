//
// Created by alexander on 07.12.16.
//

#ifndef PROXY_TOO_MANY_DATA_EXCEPTION_H
#define PROXY_TOO_MANY_DATA_EXCEPTION_H

#include <exception>
#include <string>

class too_much_data_exception : public std::exception {
    std::string error;
public:
    too_much_data_exception(std::string string) : error(string) {

    }

    too_much_data_exception & operator= (const too_much_data_exception & except) = delete;
    too_much_data_exception() = delete;

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override {
        return error.c_str();
    }

    virtual ~too_much_data_exception() {

    }
};

#endif //PROXY_TOO_MANY_DATA_EXCEPTION_H
