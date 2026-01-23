#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <vector>
#include <poll.h>
#include <sstream>
#include <iostream>

class Client
{
	private:
		int	_fd;
		std::string _buffer;
		std::string _nick;
		std::string _user;
		std::string	_ip;

		bool _registered;
		bool _hasPass;


	public:
	
		Client(int fd);
		~Client	();
		Client &operator=(Client const &src);


		int	getFd() const;
		Client* getClient(int fd);
		std::string getBuffer() const;
		std::string  getIp() const;
		const std::string &getNick() const;
		const std::string &getUser() const;

		bool isRegistered() const;
		bool hasPass() const;

		void sendMessage(const std::string &msg);

		void setIpAdd(const std::string &ip);
		void setBuffer(const std::string &buffer);
		void setNick(const std::string &nick);
		void setUser(const std::string &user);
		void setHasPass(bool v);
		void setRegistered(bool v);
		

		//buffer
		std::vector<std::string> extractLines();
		

};

#endif