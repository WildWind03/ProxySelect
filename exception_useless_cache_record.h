//
// Created by alexander on 12.12.16.
//

#ifndef PROXY_EXCEPTION_USELESS_CACHE_RECORD_H
#define PROXY_EXCEPTION_USELESS_CACHE_RECORD_H
#include <exception>
#include <string>

class exception_useless_cache_record : public std::exception {
    std::string error;
public:
    exception_useless_cache_record(std::string string) : error(string) {

    }

    exception_useless_cache_record & operator= (const exception_useless_cache_record & except) = delete;
    exception_useless_cache_record() = delete;

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override {
        return error.c_str();
    }

    virtual ~exception_useless_cache_record() {

    }
};
#endif //PROXY_EXCEPTION_USELESS_CACHE_RECORD_H
