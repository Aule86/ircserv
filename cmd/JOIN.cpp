#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Errors.hpp"
#include "../includes/Channel.hpp"
#include <sstream>

// Comando IRC para unirse a un canal
void Server::handleJOIN(Client *cli, std::istringstream &iss)
{
	std::string channelName;
	iss >> channelName;

	std::string key;
	std::getline(iss, key);
	if (!key.empty() && key[0] == ' ')
		key = key.substr(1);
	if (channelName.empty() || channelName[0] != '#')
	{
		std::string err = ":server 403 " + cli->getNick() + " " + channelName + " :No such channel\r\n";
		send(cli->getFd(), err.c_str(), err.length(), 0);
		return;
	}

	Channel *ch = getChannel(channelName);
	if (ch && ch->isInviteOnly() && !ch->isInvited(cli))
	{
		// TODO: Aquí deberías verificar si el cliente fue invitado
		// Por ahora, simplemente rechazamos el JOIN
		std::string err = ":server 473 " + cli->getNick() + " " + channelName + " :Cannot join channel (+i)\r\n";
		send(cli->getFd(), err.c_str(), err.length(), 0);
		return;
	}

	// Verificar límite de usuarios
	if (ch && ch->hasUserLimit())
	{
		size_t currentUsers = ch->getClients().size();
		size_t maxUsers = ch->getUserLimit();
		
		if (currentUsers >= maxUsers)
		{
			std::string err = ":server 471 " + cli->getNick() + " " + channelName + " :Cannot join channel (+l)\r\n";
			send(cli->getFd(), err.c_str(), err.length(), 0);
			return;
		}
	}
	if (ch && ch->isKeyActive())
	{
		if (key.empty() || key != ch->getKey())
		{
			std::string err = ":server 475 " + cli->getNick() + " " + channelName + " :Cannot join\r\n";
			send(cli->getFd(), err.c_str(), err.length(), 0);
			return ;
		}
	}

	if (!ch)
		ch = createChannel(channelName, cli);

	ch->addClient(cli);

	// Notificar al cliente que hizo JOIN
	std::string joinMsg = ":" + cli->getPrefix() + " JOIN " + channelName + "\r\n";
	ch->broadcast(joinMsg);

	std::cout << "Client " << cli->getNick() << " joined " << channelName << std::endl;
}
