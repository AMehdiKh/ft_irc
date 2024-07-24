/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziyani <aziyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:04:46 by aziyani           #+#    #+#             */
/*   Updated: 2024/07/15 18:13:39 by aziyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/Server.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cerr << "Usage: " << av[0] << " <port>" << std::endl;
        return 1;
    }
    int port = std::stoi(av[1]);
    std::string password = av[2];
    Server server(port, password);
    server.run();
    return 0;
}