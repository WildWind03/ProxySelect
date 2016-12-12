//
// Created by alexander on 12.12.16.
//

#ifndef PROXY_EXCEPTION_CONNECTION_CLOSED_WHILE_RECEIVING_RESPONSE_H
#define PROXY_EXCEPTION_CONNECTION_CLOSED_WHILE_RECEIVING_RESPONSE_H


#include <exception>
#include <string>

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

#endif //PROXY_EXCEPTION_CONNECTION_CLOSED_WHILE_RECEIVING_RESPONSE_H
