/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziyani <aziyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:04:44 by aziyani           #+#    #+#             */
/*   Updated: 2024/07/15 18:20:35 by aziyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include <iostream>
# include <netinet/in.h>
# include <sys/socket.h>
# include <unistd.h>
# include <sstream>
# include <vector>
# include <poll.h>
# include "../include/Client.hpp"

class Server
{
    private:
        int serverSocket;
        int port;
        std::string password;
        std::vector<Client> connectedClients;
        std::vector<struct pollfd> pollfds;

    public:
        Server(int port, std::string password);
        ~Server();
        
        void run();

        struct sockaddr_in sockAddr;
        
        void creatServer();
        void acceptNewClient();
        void processClientMessages(int index);
        void handleClientCommand(Client& client, const std::string& command);
};