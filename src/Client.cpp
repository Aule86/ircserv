#include "../includes/Client.hpp"

Client::Client(int fd) : _fd(fd), registered(false) {}

Client &Client::operator=(Client const &src){
	if (this != &src){
		this->_nick = src._nick;
		this->_user = src._user;
		this->_fd = src._fd;
		this->_buffer = src._buffer;
		this->registered = src.registered;
		this->_ip = src._ip;
		this->registered = src.registered;
	}
	return *this;
}

Client::~Client()
{
	close(_fd);
}

int Client::getFd() const
{
	return _fd;
}

std::string Client::getBuffer(){return _buffer;}
std::string Client::getIp() { return _ip; }


void Client::setIpAdd(const std::string &ip) { _ip = ip; }
void Client::setBuffer(const std::string &buffer) { _buffer += buffer; }



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




