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
    if (listen(this->serverSocket, 5) < 0)
    {
        std::cerr << "Error: listen failed" << std::endl;
        exit(1);
    }
}

void Server::run()
{
    pollfds.push_back({serverSocket, POLLIN, 0 });
    while(1)
    {
        int pollCount = poll(&pollfds[0], pollfds.size(), -1);
        if (pollCount < 0)
        {
            std::cerr << "Poll failed" << std::endl;
            continue;
        }
        for (size_t i = 0; i < pollfds.size(); ++i)
        {
            if (pollfds[i].revents & POLLIN)
            {
                if (pollfds[i].fd == serverSocket) 
                {
                    acceptNewClient();
                }
                else 
                {
                    processClientMessages(i);
                }
            }
        }
        
    }
}

void Server::acceptNewClient()
{
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket < 0)
    {
        std::cerr << "Accept failed" << std::endl;
        return;
    }

    connectedClients.push_back(Client(clientSocket, clientAddr));
    pollfds.push_back({ clientSocket, POLLIN, 0 });

    std::cout << "New client connected: " << inet_ntoa(clientAddr.sin_addr) << std::endl;
}

void Server::processClientMessages(int index)
{
    Client& client = connectedClients[index - 1];
    std::string message = client.receiveMessage();
    if (message.empty()) {
        close(client.getSocket());
        connectedClients.erase(connectedClients.begin() + (index - 1));
        pollfds.erase(pollfds.begin() + index);
        std::cout << "Client disconnected" << std::endl;
    } else {
        std::cout << "Received from " << client.getAddress() << ": " << message << std::endl;
        handleClientCommand(client, message);
    }
}

void Server::handleClientCommand(Client& client, const std::string& command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "NICK") {
        std::string nickname;
        iss >> nickname;
        client.setNickname(nickname);
        std::cout << "Client " << client.getAddress() << " set nickname to " << nickname << std::endl;
    } else if (cmd == "USER") {
        std::string username;
        iss >> username;
        client.setUsername(username);
        std::cout << "Client " << client.getAddress() << " set username to " << username << std::endl;
    } else if (cmd == "JOIN") {
        std::string channel;
        iss >> channel;
        client.joinChannel(channel);
        std::cout << "Client " << client.getAddress() << " joined channel " << channel << std::endl;
    } else if (cmd == "PART") {
        std::string channel;
        iss >> channel;
        client.leaveChannel(channel);
        std::cout << "Client " << client.getAddress() << " left channel " << channel << std::endl;
    } else if (cmd == "PRIVMSG") {
        std::string target;
        std::string message;
        iss >> target;
        std::getline(iss, message);
        message = message.substr(1); // Remove leading space
        client.sendMessage(target, message);
        std::cout << "Client " << client.getAddress() << " sent message to " << target << ": " << message << std::endl;
    }
}


