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
	pollfd serverPollfd = { serverSocket, POLLIN, 0 };
    pollfds.push_back(serverPollfd);
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

	struct pollfd client_fd;
	client_fd.fd = clientSocket;
	client_fd.events = POLLIN;
	client_fd.revents = 0;
	pollfds.push_back(client_fd);
	std::cout << "New client connected: " << inet_ntoa(clientAddr.sin_addr) << std::endl;
	// Prompt for password
    const char *promptMsg = "Enter password: ";
    send(clientSocket, promptMsg, strlen(promptMsg), 0);
}

bool Server::authenticateClient(int clientSocket)
{
	char buffer[BUFFER_SIZE] = {0};
	std::memset(buffer, 0, BUFFER_SIZE);
	int valread = read(clientSocket, buffer, BUFFER_SIZE -1);
	if (valread < 0)
	{
		std::cerr << "Error reading from client" << std::endl;
		return false;
	}
	std::string passwordAttempt(buffer);
	if (passwordAttempt == password)
	{
		std::cout << "Client authenticated" << std::endl;
		return true;
	}
	else
	{
		std::cout << "Client failed to authenticate" << std::endl;
		send(clientSocket, "Incorrect password", 17, 0);
		return false;
	}
}

void Server::processClientMessages(int index)
{
	int clientSocket = pollfds[index].fd;

	std::vector<int>::iterator it = std::find(authenticatedClients.begin(), authenticatedClients.end(), clientSocket);
    if (it == authenticatedClients.end())
	{
        if (authenticateClient(clientSocket))
		{
            authenticatedClients.push_back(clientSocket);
            const char *successMsg = "Authentication successful\n";
            send(clientSocket, successMsg, strlen(successMsg), 0);
        }
		else
            removeClient(index);
        return;
    }

    char buffer[BUFFER_SIZE];
    std::memset(buffer, 0, BUFFER_SIZE);
    ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);

    if (bytesRead <= 0)
	{
        if (bytesRead < 0)
            std::cerr << "Error reading from client: " << strerror(errno) << std::endl;
        removeClient(index);
        return;
    }

    std::cout << "Received message from client " << clientSocket << ": " << buffer << std::endl;
    ssize_t bytesWritten = send(clientSocket, buffer, bytesRead, 0);
    if (bytesWritten < 0)
	{
        std::cerr << "Error writing to client: " << strerror(errno) << std::endl;
        removeClient(index);
    }
}

void Server::removeClient(int index) {
    int clientSocket = pollfds[index].fd;
    close(clientSocket);
    pollfds.erase(pollfds.begin() + index);
    std::cout << "Client " << clientSocket << " disconnected" << std::endl;

    // Remove from authenticated clients list
    authenticatedClients.erase(std::remove(authenticatedClients.begin(), authenticatedClients.end(), clientSocket), authenticatedClients.end());
}

void Server::run()
{
	struct pollfd server_fd;
	server_fd.fd = serverSocket;
	server_fd.events = POLLIN;
	server_fd.revents = 0;

	pollfds.push_back(server_fd);
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