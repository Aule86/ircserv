#include "../includes/Client.hpp"

Client::Client(int fd) : _fd(fd), _registered(false), _hasPass(false) {}

Client &Client::operator=(Client const &src){
	if (this != &src){
		this->_nick = src._nick;
		this->_user = src._user;
		this->_fd = src._fd;
		this->_buffer = src._buffer;
		this->_ip = src._ip;
		this->_registered = src._registered;
		this->_hasPass = src._hasPass;
	}
	return *this;
}

Client::~Client()
{
	close(_fd);
}

//getters
int Client::getFd() const {	return _fd; }
std::string Client::getBuffer() const { return _buffer; }
std::string Client::getIp() const { return _ip; }
const std::string &Client::getNick() const { return _nick; }
const std::string &Client::getUser() const { return _user; }
bool Client::isRegistered() const { return _registered; }
bool Client::hasPass() const { return _hasPass; }

//setters
void Client::setIpAdd(const std::string &ip) { _ip = ip; }
void Client::setBuffer(const std::string &buffer) { _buffer += buffer; }
void Client::setNick(const std::string &nick) { _nick = nick; }
void Client::setUser(const std::string &user) { _user = user; }
void Client::setHasPass(bool v) { _hasPass = v; }
void Client::setRegistered(bool v) { _registered = v; }


std::vector<std::string> Client::extractLines()
{
	std::vector<std::string>	lines;
	size_t	pos;

	while((pos = _buffer.find("\n")) != std::string::npos)
	{
		std::string line = _buffer.substr(0, pos);
		if (!line.empty() && line.back() == '\r')
			line.pop_back();

		lines.push_back(line);
		_buffer.erase(0, pos + 1);
	}
	return lines;
}


