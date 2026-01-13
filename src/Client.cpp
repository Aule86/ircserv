#include "../includes/Client.hpp"

Client::Client(int fd) : _fd(fd), registered(false) {}

Client::~Client()
{
	close(_fd);
}

int Client::getFd() const
{
	return _fd;
}

bool Client::receiveData()
{
	char	tmp[512];
	ssize_t	bytes = recv(_fd, tmp, sizeof(tmp), 0);

	if (bytes <= 0)
		return false;
	
	_buffer.append(tmp, bytes);
	return true;
}

std::vector<std::string> Client::extractLines()
{
	std::vector<std::string>	lines;
	size_t	pos;

	while((pos = _buffer.find("\r\n")) != std::string::npos)
	{
		lines.push_back(_buffer.substr(0, pos));
		_buffer.erase(0, pos + 2);
	}

	return lines;
}

void Client::handdleCommand(const std::string &lines)
{
	std::istringstream iss(lines);
	std::string cmd;
	iss >> cmd;

	if (cmd == "PASS")
		iss >> _pass;
	else if (cmd == "NICK")
		iss >> _nick;
	else if (cmd == "USER")
	{
		std::string username;
		iss >> username;
		_user = username;
	}
	checkRegistration();
}

void Client::checkRegistration()
{
	if (!registered && !_nick.empty() && !_user.empty())
	{
		registered = true;

	}
}




