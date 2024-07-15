/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziyani <aziyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:04:42 by aziyani           #+#    #+#             */
/*   Updated: 2024/07/15 18:18:13 by aziyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/Server.hpp"

Server::Server(int port, std::string password)
{
    this->port = port;
    this->password = password;
    this->creatServer();
}

Server::~Server()
{
    close(this->serverSocket);
}

void Server::creatServer()
{
    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverSocket == -1)
    {
        std::cerr << "Error: socket creation failed" << std::endl;
        exit(1);
    }
    this->sockAddr.sin_family = AF_INET;
    this->sockAddr.sin_addr.s_addr = INADDR_ANY;
    this->sockAddr.sin_port = htons(this->port);
    if (bind(this->serverSocket, (struct sockaddr *)&this->sockAddr, sizeof(this->sockAddr)) == -1)
    {
        std::cerr << "Error: bind failed" << std::endl;
        exit(1);
    }
    if (listen(this->serverSocket, 1) == -1)
    {
        std::cerr << "Error: listen failed" << std::endl;
        exit(1);
    }
}

void Server::run()
{
    int clientSocket;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    while (1)
    {
        clientSocket = accept(this->serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
        if (clientSocket == -1)
        {
            std::cerr << "Error: accept failed" << std::endl;
            exit(1);
        }
        this->handleRequest(clientSocket);
    }
}

void Server::handleRequest(int clientSocket)
{
    char buffer[1024] = {0};
    int valread = read(clientSocket, buffer, 1024);
    if (valread == -1)
    {
        std::cerr << "Error: read failed" << std::endl;
        exit(1);
    }
    if (std::string(buffer) == this->password)
    {
        send(clientSocket, "Access granted", 14, 0);
    }
    else
    {
        send(clientSocket, "Access denied", 13, 0);
    }
    close(clientSocket);
}

