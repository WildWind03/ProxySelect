#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "poll.h"
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <csignal>

#include "proxy_server.h"
#include "proxy_not_created_exception.h"

//int proxy_server::socketFdStatic;

proxy_server::proxy_server(int port) {
    is_stop = false;

    http_request = (char*) malloc(MAX_CAPACITY_OF_HTTP_REQUEST);
    //parser = (http_parser*) malloc(sizeof(http_parser));
    //http_parser_settings1 = (http_parser_settings*) malloc(sizeof(http_parser_settings));

    //http_parser_init(parser, HTTP_REQUEST);

    //parser->data = this;
    //http_parser_settings1->on_header_field = my_header_field_callback_static;
    //http_parser_settings1->on_url = my_url_callback_static;

    if (port <= 0 || port > MAX_VALUE_FOR_PORT) {
        throw std::invalid_argument("Port must be positive and less than " + MAX_VALUE_FOR_PORT);
    }

    this -> port = port;
    socketFd = socket(AF_INET, SOCK_STREAM, 0);

    int flag = 1;
    setsockopt(socketFd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(int));

    //proxy_server::socketFdStatic = socketFd;
    //signal(SIGINT, proxy_server::handle_kill_static);

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
    //free(parser);
    //free(http_parser_settings1);
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
                sockets_we_wait_for_request.insert(std::pair<int, inet_socket_address>(newFd, inet_socket_address(sockaddr_in1.sin_port, inet_ntoa (sockaddr_in1.sin_addr))));
            }
        }

        for (int i = 1; i < count_of_clients + 1; i++) {
            if (poll_fds[i].revents == POLLIN) {
                ssize_t count_of_received_bytes = recv(poll_fds[i].fd, http_request, MAX_CAPACITY_OF_HTTP_REQUEST, 0);
                std::cout << http_request << std::endl;
                //http_parser_execute(parser, http_parser_settings1, http_request, (size_t) count_of_received_bytes);
                close(poll_fds[i].fd);
                sockets_we_wait_for_request.erase(poll_fds[i].fd);
            }
        }

        free(poll_fds);
    }
}

/*int proxy_server::my_header_field_callback(http_parser *parser1, const char *header, size_t size) {
    if (parser1 -> http_major != 1 || parser1 -> http_minor != 0) {
        std::cerr << "The version is not supported: " << parser1 -> http_major << "." << parser1 -> http_minor << std::endl;
        return 0;
    }

    if (parser1 -> method != HTTP_GET) {
        std::cerr << "The request is not a GET request" << std::endl;
        return 0;
    }


    /*std::cout << "Handle get request with appropriate http version" << std::endl;
    std::cout << header << std::endl;

    http_parser_url http_parser_url1;
    http_parser_url_init(&http_parser_url1);
    http_parser_parse_url()
     */


    //std::cout << "Handle new byte message! Header: " << header << std::endl;

    //return 0;
//}

void proxy_server::stop() {
    is_stop = true;
}

/*int proxy_server::my_header_field_callback_static(http_parser *parser1, const char *header, size_t size) {
    proxy_server *proxyServer = (proxy_server*) parser1 -> data;
    return proxyServer->my_header_field_callback(parser1, header, size);
}

void proxy_server::handle_kill_static(int sigNum) {
    close(socketFdStatic);
}

int proxy_server::my_url_callback_static(http_parser *parser1, const char *url, size_t size) {
    proxy_server *proxyServer = (proxy_server*) parser1 -> data;
    return proxyServer -> my_url_callback(parser1, url, size);
}

int proxy_server::my_url_callback(http_parser *parser1, const char *url, size_t size) {
    if (parser1 -> http_major != 1 || parser1 -> http_minor != 0) {
        std::cerr << "The version is not supported: " << parser1 -> http_major << "." << parser1 -> http_minor << std::endl;
        return 0;
    }

    if (parser1 -> method != HTTP_GET) {
        std::cerr << "The request is not a GET request" << std::endl;
        return 0;
    }

    return 0;
}
 */





