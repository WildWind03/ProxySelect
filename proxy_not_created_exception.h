//
// Created by alexander on 01.12.16.
//

#ifndef PROXY_PROXYNOTCREATEDEXCEPTION_H
#define PROXY_PROXYNOTCREATEDEXCEPTION_H

#include <exception>
#include <string>

class proxy_not_created_exception : public std::exception {
    std::string errorStr;
public:
    proxy_not_created_exception(std::string string) : errorStr(string) {

    }

    proxy_not_created_exception & operator= (const proxy_not_created_exception & except) = delete;
    proxy_not_created_exception() = delete;

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override {
        return errorStr.c_str();
    }

    virtual ~proxy_not_created_exception() {

    }
};
#endif //PROXY_PROXYNOTCREATEDEXCEPTION_H
