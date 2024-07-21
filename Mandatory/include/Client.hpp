# pragma once

# include <string>
# include <unistd.h>
# include <netinet/in.h>
# include <arpa/inet.h>

class Client
{
	private:
		int socket;
		struct sockaddr_in _addr;
		std::string nickname;
		std::string username;
		std::string currentChannel;

	public:
		Client(int socket, struct sockaddr_in addr);
		~Client();

		int getSocket() const;
		std::string getAddress() const;

		void setNickname(const std::string& nickname);
		void setUsername(const std::string& username);
		void joinChannel(const std::string& channel);
		void leaveChannel(const std::string& channel);
		void sendMessage(const std::string& target, const std::string& message);

		std::string receiveMessage();
};
