#ifndef CLIENT_HPP
#define CLIENT_HPP

//#include "Server.hpp"
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

	public:
		std::string _nick;
		std::string _pass;
		std::string _user;
		bool registered;
	
		Client(int fd);
		~Client	();

		int	getFd() const;

		//buffer
		bool receiveData();
		std::vector<std::string> extractLines();

		//irc commands
		void handdleCommand(const std::string &line);
		void checkRegistration();

};

#endif