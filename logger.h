//
// Created by alexander on 10.12.16.
//

#ifndef PROXY_LOGGER_H
#define PROXY_LOGGER_H

#include <string>
#include <iostream>

class logger {
    std::string name;

public:
    logger(std::string name) {
        this -> name = "[" + name + "] ";
    }

    void log(std::string string) {
        std::cout << name << string << std::endl;
    }

    void add_name(std::string string) {
        this -> name = this -> name + "[" + string + "] ";
    }
};

#endif //PROXY_LOGGER_H
