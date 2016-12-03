#include <iostream>
#include "proxy_server.h"
#include "proxy_not_created_exception.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    try {
        proxy_server proxy_server1(12000);
        proxy_server1.start();
    } catch (proxy_not_created_exception & e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}