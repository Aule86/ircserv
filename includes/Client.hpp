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
		Client(int fd);
		~Client	();

		int	getFd() const;
		std::string &getBuffer();
};

#endif