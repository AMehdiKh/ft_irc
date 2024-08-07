#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
public:
    Client(int fd);
    int getFd() const;
    std::string getNickname() const;
    void setNickname(const std::string &nickname);

private:
    int fd;
    std::string nickname;
    std::string username;
};

#endif
