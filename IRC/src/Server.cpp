#include "Server.hpp"
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "Utils.hpp"

Server::Server(int port, const std::string &password) 
    : port(port), password(password), serverSocket(-1) {
    setupServer();
}

void Server::setupServer() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        throw std::runtime_error("Failed to bind socket");
    }

    if (listen(serverSocket, 5) < 0) {
        throw std::runtime_error("Failed to listen on socket");
    }

    pollfd serverPollFd = {serverSocket, POLLIN, 0};
    pollFds.push_back(serverPollFd);
}

void Server::run() {
    while (true) {
        if (poll(pollFds.data(), pollFds.size(), -1) < 0) {
            throw std::runtime_error("Poll failed");
        }

        for (size_t i = 0; i < pollFds.size(); ++i) {
            if (pollFds[i].revents & POLLIN) {
                if (pollFds[i].fd == serverSocket) {
                    handleNewConnection();
                } else {
                    handleClientMessage(pollFds[i].fd);
                }
            }
        }
    }
}

void Server::handleNewConnection() {
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket < 0) {
        std::cerr << "Failed to accept new connection" << std::endl;
        return;
    }

    pollfd clientPollFd = {clientSocket, POLLIN, 0};
    pollFds.push_back(clientPollFd);
    clients[clientSocket] = Client(clientSocket);
}

void Server::handleClientMessage(int clientFd) {
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    int bytesReceived = recv(clientFd, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0) {
        removeClient(clientFd);
        return;
    }

    std::string message(buffer);
    std::vector<std::string> tokens = split(message, ' ');

    if (tokens[0] == "NICK") {
        clients[clientFd].setNickname(tokens[1]);
    }
    // Handle other commands like JOIN, PRIVMSG, etc.
}

void Server::removeClient(int clientFd) {
    close(clientFd);
    pollFds.erase(std::remove_if(pollFds.begin(), pollFds.end(), [clientFd](const pollfd &pfd) {
        return pfd.fd == clientFd;
    }), pollFds.end());
    clients.erase(clientFd);
}
