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

class Server
{
    private:
        int serverSocket;
        int port;
        std::string password;


    public:
        Server(int port, std::string password);
        ~Server();
        
        void run();

        struct sockaddr_in sockAddr;
        
        void creatServer();
        void handlRequest();

};