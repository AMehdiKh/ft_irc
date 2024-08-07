#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <set>

class Channel {
public:
    Channel(const std::string &name);
    void addClient(int clientFd);
    void removeClient(int clientFd);
    const std::set<int> &getClients() const;

private:
    std::string name;
    std::set<int> clients;
};

#endif
