//
// Created by alexander on 01.12.16.
//

#ifndef PROXY_PROXYNOTCREATEDEXCEPTION_H
#define PROXY_PROXYNOTCREATEDEXCEPTION_H

#include <exception>
#include <string>

class ProxyNotCreatedException : std::exception {
    std::string errorStr;
public:
    ProxyNotCreatedException(std::string string) : errorStr(string) {

    }

    ProxyNotCreatedException & operator= (const ProxyNotCreatedException & except) = delete;
    ProxyNotCreatedException() = delete;

    virtual ~BannedActionException() {

    }
};
#endif //PROXY_PROXYNOTCREATEDEXCEPTION_H
