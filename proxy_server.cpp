#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "poll.h"
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <csignal>
#include <netdb.h>

#include "proxy_server.h"
#include "proxy_not_created_exception.h"
#include "http_parser.h"

//int proxy_server::socketFdStatic;

proxy_server::proxy_server(int port) {
    is_stop = false;

    http_request = (char*) malloc(MAX_CAPACITY_OF_HTTP_REQUEST);

    if (port <= 0 || port > MAX_VALUE_FOR_PORT) {
        throw std::invalid_argument("Port must be positive and less than " + MAX_VALUE_FOR_PORT);
    }

    this -> port = port;
    socketFd = socket(AF_INET, SOCK_STREAM, 0);

    int flag = 1;
    setsockopt(socketFd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(int));


    if (-1 == socketFd) {
        throw proxy_not_created_exception("Socket system call return error");
    }

    std::cout << "Socket is created!" << std::endl;

    struct sockaddr_in sockaddr_in1;
    sockaddr_in1.sin_family = AF_INET;
    sockaddr_in1.sin_port = htons(port);
    sockaddr_in1.sin_addr.s_addr = inet_addr(BIND_IP);

    int result_of_binding = bind(socketFd, (struct sockaddr *) &sockaddr_in1, sizeof(sockaddr_in1));
    if (0 != result_of_binding) {
        throw proxy_not_created_exception("Can't bind socket");
    }

    std::cout << "The socket is binded!" << std::endl;

    int result_of_listening_socket = listen(socketFd, MAX_COUNT_OF_PENDING_REQUESTS);
    if (0 != result_of_listening_socket) {
        throw new proxy_not_created_exception("Can't listen on the socket");
    }

    std::cout << "Socket started listening" << std::endl;
}

proxy_server::~proxy_server() {
    free(http_request);
}

void proxy_server::start() {
    std::cout << "Proxy server start working" << std::endl;

    while (!is_stop) {
        size_t count_of_clients = sockets_we_wait_for_request.size();

        struct pollfd * poll_fds = (struct pollfd*) malloc(sizeof(struct pollfd*) * (count_of_clients + 1));
        poll_fds[0].events = POLLIN;
        poll_fds[0].fd = socketFd;

        for (auto & iter : sockets_we_wait_for_request) {
            poll_fds[1].events = POLLIN;
            poll_fds[1].fd = iter.first;
        }

        while (0 == poll(poll_fds, count_of_clients + 1, TIME_TO_BE_BLOCKED_IN_POLL)) {
            if (is_stop) {
                free(poll_fds);
                return;
            }
        }

        if (poll_fds[0].revents == POLLIN) {
            struct sockaddr_in sockaddr_in1;
            socklen_t socklen = sizeof(sockaddr_in1);

            int newFd = accept(socketFd, (struct sockaddr*) &sockaddr_in1, &socklen);
            if (-1 == newFd) {
                std::cout << "Can't accept new connection" << std::endl;
            } else {
                std::cout << "New connection accepted!" << std::endl;
                sockets_we_wait_for_request.insert(std::pair<int, client>(newFd, client(sockaddr_in1.sin_port, inet_ntoa (sockaddr_in1.sin_addr))));
            }
        }

        for (int i = 1; i < count_of_clients + 1; i++) {
            if (poll_fds[i].revents == POLLIN) {
                client client1 = sockets_we_wait_for_request.find(poll_fds[i].fd).operator*().second;
                size_t max_size_to_read = client1.get_max_data_to_read();

                ssize_t count_of_received_bytes = recv(poll_fds[i].fd, http_request, MAX_CAPACITY_OF_HTTP_REQUEST, 0);

                if (count_of_received_bytes > max_size_to_read) {
                    std::cout << "Too much data" << std::endl;
                    close(poll_fds[i].fd);
                    sockets_we_wait_for_request.erase(poll_fds[i].fd);
                }

                if (0 == count_of_received_bytes) {
                    std::cout << "The connection is closed!" << std::endl;
                    close(poll_fds[i].fd);
                    sockets_we_wait_for_request.erase(poll_fds[i].fd);
                } else {
                    client1.add_new_data(http_request, (size_t) count_of_received_bytes);

                    if (client1.is_correct_request()) {
                        std::cout << "The request is got" << std::endl;

                        http_parser http_parser1(client1.get_request().c_str());

                        int major_v = http_parser1.get_major_version();
                        int minor_v = http_parser1.get_minor_version();

                        if (major_v != 1 || minor_v != 0) {
                            std::cout << "The version of http protocol is not supported" << std::endl;
                        } else {
                            int request_type = http_parser1.get_request_type();
                            switch (request_type) {
                                case http_parser::GET_REQUEST :
                                    onGetRequestReceived(http_parser1.get_host(), client1.get_request());
                                    break;
                                default:
                                    std::cout << "The type of request is not supported" << std::endl;
                                    break;
                            }
                        }

                        close(poll_fds[i].fd);
                        sockets_we_wait_for_request.erase(poll_fds[i].fd);
                    }
                }
            }
        }

        free(poll_fds);
    }
}

void proxy_server::stop() {
    is_stop = true;
}

void proxy_server::onGetRequestReceived(std::string host, std::string request) {
    std::cout << host.c_str() << std::endl;
    addrinfo *res;
    getaddrinfo(host.c_str(), NULL, NULL, &res);
    char ip_address[30];
    getnameinfo(res->ai_addr, res->ai_addrlen, ip_address, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
    std::cout << ip_address << std::endl;
}

int proxy_server::hostname_to_ip(char *ip, const char *hostname) {
    return 0;
}





