//
// Created by alexander on 01.12.16.
//

#ifndef PROXY_PROXYNOTCREATEDEXCEPTION_H
#define PROXY_PROXYNOTCREATEDEXCEPTION_H

#include <exception>
#include <string>

class exception_proxy_not_created : public std::exception {
    std::string errorStr;
public:
    exception_proxy_not_created(std::string string) : errorStr(string) {

    }

    exception_proxy_not_created & operator= (const exception_proxy_not_created & except) = delete;
    exception_proxy_not_created() = delete;

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override {
        return errorStr.c_str();
    }

    virtual ~exception_proxy_not_created() {

    }
};
#endif //PROXY_PROXYNOTCREATEDEXCEPTION_H
