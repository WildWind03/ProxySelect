//
// Created by alexander on 12.12.16.
//

#ifndef PROXY_EXCEPTION_CONNECTION_CLOSED_WHILE_RECEIVING_RESPONSE_H
#define PROXY_EXCEPTION_CONNECTION_CLOSED_WHILE_RECEIVING_RESPONSE_H


#include <exception>
#include <string>

class exception_connection_closed_while_receiving_response : public std::exception {
    std::string error;

public:
    exception_connection_closed_while_receiving_response(std::string string) : error(string) {

    }

    exception_connection_closed_while_receiving_response & operator= (const exception_connection_closed_while_receiving_response & except) = delete;
    exception_connection_closed_while_receiving_response() = delete;

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override {
        return error.c_str();
    }

    virtual ~exception_connection_closed_while_receiving_response() {

    }
};

#endif //PROXY_EXCEPTION_CONNECTION_CLOSED_WHILE_RECEIVING_RESPONSE_H
