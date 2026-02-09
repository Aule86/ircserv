#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include <iostream>

Channel::Channel(const std::string& name) : _name(name), _topic(""), _inviteOnly(false), _keyActive(false) {}

Channel::~Channel()
{
	_clients.clear();
	_operators.clear();
	_invited.clear();
}

const std::string& Channel::getName() const
{
	return _name;
}

bool Channel::hasClient(Client* client) const
{
	return (_clients.find(client->getFd()) != _clients.end());
}

void Channel::addClient(Client* client)
{
	int fd = client->getFd();

	if (hasClient(client))
		return;

	_clients[fd] = client;
}

void Channel::removeClient(Client* client)
{
	int fd = client->getFd();

	_clients.erase(fd);
	_operators.erase(fd);
}

void Channel::addOperator(Client* client)
{
	_operators.insert(client->getFd());
}

bool Channel::isOperator(Client* client) const
{
	return (_operators.find(client->getFd()) != _operators.end());
}

// Envía un mensaje a todos los clientes del canal
void Channel::broadcast(const std::string& msg, Client* except)
{
	for (std::map<int, Client*>::iterator it = _clients.begin();
		it != _clients.end();
		++it)
	{
		if (except && it->second == except)
			continue;

		it->second->sendMessage(msg); 
	}
}

const std::map<int, Client*>& Channel::getClients() const
{
	return _clients;
}

bool Channel::isInviteOnly() const
{
	return _inviteOnly;
}

// Establece si el canal es solo por invitación
void Channel::setInviteOnly(bool value)
{
	_inviteOnly = value;
}

bool Channel::isKeyActive() const
{ 
	return _keyActive;
}

void Channel::setKeyActive(bool value)
{
	_keyActive = value;
	if (!value)
		_key.clear();
}

void Channel::setKey(const std::string &key)
{
	_key = key;
	_keyActive = true;
}

const std::string &Channel::getKey() const
{
	return _key;
}

void Channel::addInvited(Client* client)
{
	_invited.insert(client->getFd());
}

bool Channel::isInvited(Client* client) const
{
	return (_invited.find(client->getFd()) != _invited.end());
}

void Channel::removeInvited(Client* client)
{
	_invited.erase(client->getFd());
}

// ========== LÍMITE DE USUARIOS ==========

bool Channel::hasUserLimit() const
{
	return _hasUserLimit;
}

size_t Channel::getUserLimit() const
{
	return _userLimit;
}

void Channel::setUserLimit(size_t limit)
{
	_hasUserLimit = true;
	_userLimit = limit;
}

void Channel::removeUserLimit()
{
	_hasUserLimit = false;
	_userLimit = 0;
}
