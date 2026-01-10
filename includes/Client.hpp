#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include <unistd.h>

class Client
{
	private:
		int	_fd;
		std::string _buffer;
		

	public:
		std::string nick;
		std::string pass;
		std::string user;
		bool registered;
	
		Client(int fd);
		~Client	();

		int	getFd() const;
		std::string &getBuffer();
};

#endif