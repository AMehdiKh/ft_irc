#include "Channel.hpp"

Channel::Channel(const std::string &name) : name(name) {}

void Channel::addClient(int clientFd) {
    clients.insert(clientFd);
}

void Channel::removeClient(int clientFd) {
    clients.erase(clientFd);
}

const std::set<int>& Channel::getClients() const {
    return clients;
}
