#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <poll.h>
#include "Client.hpp"
#include "Channel.hpp"

class Server {
public:
    Server(int port, const std::string &password);
    void run();

private:
    int port;
    std::string password;
    int serverSocket;
    std::vector<pollfd> pollFds;
    std::map<int, Client> clients;
    std::map<std::string, Channel> channels;

    void setupServer();
    void handleNewConnection();
    void handleClientMessage(int clientFd);
    void removeClient(int clientFd);
};

#endif
