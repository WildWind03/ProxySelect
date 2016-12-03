//
// Created by alexander on 01.12.16.
//

#ifndef PROXY_INETSOCKETADDRESS_H
#define PROXY_INETSOCKETADDRESS_H

#include <string>


class inet_socket_address {
    int port;
    std::string ip;
public:
    inet_socket_address(int port, std::string ip) {
        this->ip = ip;
        this->port = port;
    }

    int getPort() {
        return port;
    }

    std::string getIP() {
        return ip;
    }

};


#endif //PROXY_INETSOCKETADDRESS_H
