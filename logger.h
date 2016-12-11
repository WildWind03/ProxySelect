//
// Created by alexander on 10.12.16.
//

#ifndef PROXY_LOGGER_H
#define PROXY_LOGGER_H

#include <string>
#include <iostream>
#include <fstream>

class logger {
    std::string name;
    std::string filename;

    const std::string DEFAULT_NAME = "DEFAULT";

public:
    logger(std::string name) {
        this -> name = "[" + name + "] ";
        this -> filename = DEFAULT_NAME;
    }

    logger(std::string name, std::string filename)  {
        this -> name = "[" + name + "] ";
        this -> filename = filename;
    }


    void log(std::string string) {
        if (DEFAULT_NAME == filename) {
            std::cout << name << string << std::endl;
        } else {
            std::ofstream ofstream1(filename, std::ios_base::app);
            ofstream1 << name << string << std::endl;
            ofstream1.close();
        }
    }

    void add_name(std::string string) {
        this -> name = this -> name + "[" + string + "] ";
    }
};

#endif //PROXY_LOGGER_H
