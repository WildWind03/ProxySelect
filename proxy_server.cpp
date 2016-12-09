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
#include "exception_proxy_not_created.h"

proxy_server::proxy_server(int port) {
    if (port <= 0 || port > MAX_VALUE_FOR_PORT) {
        throw std::invalid_argument("Port must be positive and less than " + MAX_VALUE_FOR_PORT);
    }

    this -> port = port;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    int flag = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));

    if (-1 == socket_fd) {
        throw exception_proxy_not_created("Socket system call return error");
    }

    std::cout << "Socket is created!" << std::endl;

    struct sockaddr_in sockaddr_in1;
    sockaddr_in1.sin_family = AF_INET;
    sockaddr_in1.sin_port = htons(port);
    sockaddr_in1.sin_addr.s_addr = inet_addr(BIND_IP);

    int result_of_binding = bind(socket_fd, (struct sockaddr *) &sockaddr_in1, sizeof(sockaddr_in1));
    if (0 != result_of_binding) {
        throw exception_proxy_not_created("Can't bind socket");
    }

    std::cout << "The socket is binded!" << std::endl;

    int result_of_listening_socket = listen(socket_fd, MAX_COUNT_OF_PENDING_REQUESTS);
    if (0 != result_of_listening_socket) {
        throw new exception_proxy_not_created("Can't listen on the socket");
    }

    std::cout << "Socket started listening" << std::endl;
}

proxy_server::~proxy_server() {
    for (auto & iter : requests) {
        free(iter.second);
    }

    for (auto & iter : storage) {
        free (iter.second);
    }
}

void proxy_server::start() {
    std::cout << "Proxy server start working" << std::endl;

    while (!is_stop) {

        size_t count_of_clients = 0;
        for (auto iter : requests) {
            if (iter.second->is_selectable()) {
                ++count_of_clients;
            }
        }

        struct pollfd * poll_fds = (struct pollfd*) malloc(sizeof(struct pollfd*) * (count_of_clients + 1));

        poll_fds[0].events = POLLIN;
        poll_fds[0].fd = socket_fd;

        int current_pos = 1;
        for (auto & iter : requests) {
            if (iter.second->is_selectable()) {
                poll_fds[current_pos].events = iter.second -> get_socket_select_event();
                poll_fds[current_pos].fd = iter.second -> get_socket();
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
                //std::cout << "New connection accepted!" << std::endl;
                request_base * base_request1 = new request_client(newFd, inet_ntoa (sockaddr_in1.sin_addr), sockaddr_in1.sin_port);
                requests.insert(std::pair<int, request_base*>(newFd, base_request1));
            }
        }

        for (int i = 1; i < count_of_clients + 1; i++) {
                if (poll_fds[i].revents == POLLIN || poll_fds[i].revents == POLLOUT) {
                    request_base *base_request1 = requests.find(poll_fds[i].fd).operator*().second;

                    try {
                        request_enum request_value = base_request1 -> exec();

                        switch (request_value) {
                            case request_enum::READ_FROM_CLIENT_FINISHED : {
                                std::cout << "The request is ready to be sent to the server" << std::endl;
                                onGetRequestReceived((request_client *) base_request1);
                                break;
                            }

                            case request_enum::WRITE_TO_CLIENT_FINISHED : {
                                onRequestSatisfied(poll_fds[i].fd);
                                break;
                            }

                            case request_enum::WRITE_TO_SERVER_FINISHED : {
                                std::cout << "The request was sent to server successfully and fully" << std::endl;
                                onRequestPassedToServer((request_server *) base_request1);
                                break;
                            }

                            default: {
                                break;
                            }
                        }

                    } catch (exception_connection_closed & exception) {
                        std::cout << exception.what() << std::endl;
                        close(base_request1->get_socket());
                        free(base_request1);
                        requests.erase(poll_fds[i].fd);
                    } catch (exception_invalid_http_data & exception1) {
                        std::cout << exception1.what() << std::endl;
                        close(base_request1->get_socket());
                        free(base_request1);
                        requests.erase(poll_fds[i].fd);
                    } catch (exception_not_supported_request & request_not_supported_exception1) {
                        std::cout << request_not_supported_exception1.what() << std::endl;
                        close(base_request1->get_socket());
                        free(base_request1);
                        requests.erase(poll_fds[i].fd);
                    }
                }
        }


        free(poll_fds);
    }
}

void proxy_server::stop() {
    is_stop = true;
}

void proxy_server::onGetRequestReceived(request_client *request_client1) {
    auto iter = storage.find(request_client1->get_url());

    cached_data *cached_data1 = nullptr;

    while (iter != storage.end()) {
        if (!iter.operator*().second -> is_streaming) {
            cached_data1 = iter.operator*().second;
            break;
        }

        iter++;
    }

    if (nullptr == cached_data1) {
        std::cout << "There is no such valid cache record in the cache" << std::endl;
        cached_data1 = new cached_data();

        int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (-1 == server_socket_fd) {
            std::cout << "Can not create socket" << std::endl;
            return;
        }

        std::string ip = hostname_to_ip(request_client1 -> get_host());

        request_server *server_request1 = new request_server(server_socket_fd, ip, HTTP_PORT, request_client1 -> get_request(), request_client1 -> get_size_of_request(), cached_data1);
        requests.insert(std::pair<int, request_base*>(server_socket_fd, server_request1));
        storage.insert(std::pair<std::string, cached_data*> (request_client1->get_url(), cached_data1));

        cached_data1->add_new_observer(server_request1, server_socket_fd);
        cached_data1->add_server_observer(server_request1);
    } else {
        std::cout << "There has already been such valid cache record in the cache" << std::endl;
    }

    cached_data1 -> add_new_observer(request_client1, request_client1 -> get_socket());
    request_client1 -> change_to_write_mode(cached_data1);

    std::cout << "The request is handled successfully" << std::endl;
}

std::string proxy_server::hostname_to_ip(std::string host) {
    addrinfo *res;
    getaddrinfo(host.c_str(), NULL, NULL, &res);
    char ip_address[50];
    getnameinfo(res->ai_addr, res->ai_addrlen, ip_address, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

    return std::string(ip_address);
}

void proxy_server::onRequestSatisfied(int fd) {
    close(fd);
    free(requests.find(fd).operator*().second);
    requests.erase(fd);
}

void proxy_server::onRequestPassedToServer(request_server *request_server1) {
    request_server1 -> change_to_read_mode();
}





