#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include "proxy_server.h"
#include "proxy_not_created_exception.h"
#include "http_parser.h"

int main() {
    try {
        proxy_server proxy_server1(12000);
        proxy_server1.start();
    } catch (proxy_not_created_exception & e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}