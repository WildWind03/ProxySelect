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
#include "client_request.h"

proxy_server::proxy_server(int port) {
    is_stop = false;

    if (port <= 0 || port > MAX_VALUE_FOR_PORT) {
        throw std::invalid_argument("Port must be positive and less than " + MAX_VALUE_FOR_PORT);
    }

    this -> port = port;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    int flag = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));

    if (-1 == socket_fd) {
        throw proxy_not_created_exception("Socket system call return error");
    }

    std::cout << "Socket is created!" << std::endl;

    struct sockaddr_in sockaddr_in1;
    sockaddr_in1.sin_family = AF_INET;
    sockaddr_in1.sin_port = htons(port);
    sockaddr_in1.sin_addr.s_addr = inet_addr(BIND_IP);

    int result_of_binding = bind(socket_fd, (struct sockaddr *) &sockaddr_in1, sizeof(sockaddr_in1));
    if (0 != result_of_binding) {
        throw proxy_not_created_exception("Can't bind socket");
    }

    std::cout << "The socket is binded!" << std::endl;

    int result_of_listening_socket = listen(socket_fd, MAX_COUNT_OF_PENDING_REQUESTS);
    if (0 != result_of_listening_socket) {
        throw new proxy_not_created_exception("Can't listen on the socket");
    }

    std::cout << "Socket started listening" << std::endl;
}

proxy_server::~proxy_server() {

}

void proxy_server::start() {
    std::cout << "Proxy server start working" << std::endl;

    while (!is_stop) {

        size_t count_of_clients = 0;
        for (auto & iter : requests) {
            if (iter.second.is_active_request()) {
                ++count_of_clients;
            }
        }

        struct pollfd * poll_fds = (struct pollfd*) malloc(sizeof(struct pollfd*) * (count_of_clients + 1));

        poll_fds[0].events = POLLIN;
        poll_fds[0].fd = socket_fd;

        int current_pos = 1;
        for (auto & iter : requests) {
            if (iter.second.is_active_request()) {
                poll_fds[current_pos].events = iter.second.get_socket_select_event();
                poll_fds[current_pos].fd = socket_fd;
            }
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

            int newFd = accept(socket_fd, (struct sockaddr*) &sockaddr_in1, &socklen);
            if (-1 == newFd) {
                std::cout << "Can't accept new connection" << std::endl;
            } else {
                std::cout << "New connection accepted!" << std::endl;
                requests.insert(std::pair<int, client_request>(newFd, client_request(newFd, inet_ntoa (sockaddr_in1.sin_addr), sockaddr_in1.sin_port)));
            }
        }

        for (int i = 1; i < count_of_clients + 1; i++) {
            if (poll_fds[i].revents == POLLIN) {
                request_enum request_value = requests.find(poll_fds[i].fd).operator*().second.exec();

                switch (request_value) {
                    case request_enum::READ_FROM_CLIENT_FINISHED :
                        break;
                    case request_enum::WRITE_TO_CLIENT_FINISHED :
                        close(poll_fds[i].fd);
                        requests.erase(poll_fds[i].fd);
                        break;
                    default:
                        break;
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
    int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == server_socket_fd) {
        std::cout << "Can not create socket" << std::endl;
        return;
    }

    std::string ip = hostname_to_ip(host);

    struct sockaddr_in sockaddr_in1;
    sockaddr_in1.sin_family = AF_INET;
    sockaddr_in1.sin_port = htons(HTTP_PORT);
    sockaddr_in1.sin_addr.s_addr = inet_addr(ip.c_str());

    int connect_result = connect(server_socket_fd, (struct sockaddr *) &sockaddr_in1, sizeof(sockaddr_in1));

    if (-1 == connect_result) {
        std::cout << "Can not connect to " + ip << std::endl;
        close(server_socket_fd);
        return;
    }

    /*ssize_t count_of_send_data = send(server_socket_fd, request.c_str(), request.size(), NULL);
    if (count_of_send_data != request.size()) {
        std::cout << "Error while sending message" << std::endl;
    } else {
        std::cout << "The request was successfully sent to the server" << std::endl;

    }*/

    close(server_socket_fd);
}

std::string proxy_server::hostname_to_ip(std::string host) {
    addrinfo *res;
    getaddrinfo(host.c_str(), NULL, NULL, &res);
    char ip_address[30];
    getnameinfo(res->ai_addr, res->ai_addrlen, ip_address, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

    return std::string(ip_address);
}





