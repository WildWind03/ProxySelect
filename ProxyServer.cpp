#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "poll.h"
#include <stdexcept>

#include "ProxyServer.h"
#include "ProxyNotCreatedException.h"


ProxyServer::ProxyServer(int port) {
    if (port <= 0 || port > MAX_VALUE_FOR_PORT) {
        throw std::invalid_argument("Port must be positive and less than " + MAX_VALUE_FOR_PORT);
    }

    this -> port = port;
    socketFd = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == socketFd) {
        throw ProxyNotCreatedException("Socket system call return error");
    }

    struct sockaddr_in sockaddrIn;
    sockaddrIn.sin_family = AF_INET;
    sockaddrIn.sin_port = (in_port_t ) port;
    in_addr in_addr1;
    inet_aton(BIND_IP, &in_addr1);
    sockaddrIn.sin_addr = in_addr1;

    int resultOfBinding = bind(socketFd, (struct sockaddr *) &sockaddrIn, sizeof(sockaddrIn));
    if (0 != resultOfBinding) {
        throw new ProxyNotCreatedException("Can't bind socket");
    }

    int resultOfListenSocket = listen(socketFd, MAX_COUNT_OF_PENDING_REQUESTS);
    if (0 != resultOfListenSocket) {
        throw new ProxyNotCreatedException("Can't listen on the socket");
    }
}

ProxyServer::~ProxyServer() {

}

void ProxyServer::start() {
    while (true) {

        size_t countOfClients = sockets.size();
        struct pollfd * pollFds = (struct pollfd*) malloc(sizeof(struct pollfd*) * (countOfClients + 1));
        pollFds[0].events = POLLIN;
        pollFds[0].fd = socketFd;

        for (auto & iter : sockets) {
            pollFds[1].events = POLLIN;
            pollFds[1].fd = iter.second;
        }

        while (true) {
            int resultOfPoll = poll(pollFds, countOfClients + 1, TIME_TO_BE_BLOCKED_IN_POLL);
            if (0 == resultOfPoll) {
                continue;
            } else {
                if (pollFds[0].revents == POLLIN) {
                    struct sockaddr_in sockaddrIn;
                    socklen_t socklen;

                    int newFd = accept(socketFd, (struct sockaddr*) &sockaddrIn, &socklen);

                }

                for (int i = 1; i < countOfClients + 1; i++) {
                    if (pollFds[i].revents == POLLIN) {

                    }
                }
            }
        }


        while (ite)

        sockets.erase()
        poll()
    }
    select()
    accept()
}



