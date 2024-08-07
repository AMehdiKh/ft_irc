#include "Client.hpp"

Client::Client(int fd) : fd(fd) {}

int Client::getFd() const {
    return fd;
}

std::string Client::getNickname() const {
    return nickname;
}

void Client::setNickname(const std::string &nickname) {
    this->nickname = nickname;
}
