#include "../include/Client.hpp"

Client::Client(int socket, struct sockaddr_in addr) 
    : socket(socket), _addr(addr) {}

Client::~Client() {
    close(socket);
}

int Client::getSocket() const {
    return socket;
}

std::string Client::getAddress() const {
    return inet_ntoa(_addr.sin_addr);
}

void Client::setNickname(const std::string& nickname) {
    this->nickname = nickname;
}

void Client::setUsername(const std::string& username) {
    this->username = username;
}

void Client::joinChannel(const std::string& channel) {
    this->currentChannel = channel;
}

void Client::leaveChannel(const std::string& channel) {
    if (currentChannel == channel) {
        currentChannel.clear();
    }
}

void Client::sendMessage(const std::string& target, const std::string& message) {
    std::string fullMessage = "PRIVMSG " + target + " :" + message + "\r\n";
    send(socket, fullMessage.c_str(), fullMessage.size(), 0);
}

std::string Client::receiveMessage() {
    char buffer[1024];
    int bytesReceived = recv(socket, buffer, sizeof(buffer), 0);
    if (bytesReceived < 0) {
        return "";
    }
    buffer[bytesReceived] = '\0';
    return std::string(buffer);
}
