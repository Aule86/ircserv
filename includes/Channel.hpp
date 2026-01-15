#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include "Server.hpp"

class Channel
{
	private:

	public:
		Channel();
		~Channel();
		Channel(Channel const &src);
		Channel &operator=(Channel const &src);
};

#endif