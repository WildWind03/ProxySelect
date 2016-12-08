#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include "proxy_server.h"
#include "exception_proxy_not_created.h"
#include "http_parser.h"

int main() {
    try {
        proxy_server proxy_server1(12000);
        proxy_server1.start();
    } catch (exception_proxy_not_created & e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}