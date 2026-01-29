#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Errors.hpp"
#include "../includes/Channel.hpp"
#include <sstream>

void Server::handlePART(Client *cli, std::istringstream &iss)
{
	std::string channelName;
	iss >> channelName;

	// Validación: nombre del canal
	if (channelName.empty())
	{
		std::string err = ":server 461 " + cli->getNick() + " PART :Not enough parameters\r\n";
		send(cli->getFd(), err.c_str(), err.length(), 0);
		return;
	}

	// Validación: debe empezar con #
	if (channelName[0] != '#')
	{
		std::string err = ":server 403 " + cli->getNick() + " " + channelName + " :No such channel\r\n";
		send(cli->getFd(), err.c_str(), err.length(), 0);
		return;
	}

	// Buscar el canal
	Channel *ch = getChannel(channelName);
	if (!ch)
	{
		std::string err = ":server 403 " + cli->getNick() + " " + channelName + " :No such channel\r\n";
		send(cli->getFd(), err.c_str(), err.length(), 0);
		return;
	}

	// Verificar que el usuario está en el canal
	if (!ch->hasClient(cli))
	{
		std::string err = ":server 442 " + cli->getNick() + " " + channelName + " :You're not on that channel\r\n";
		send(cli->getFd(), err.c_str(), err.length(), 0);
		return;
	}

	// Mensaje de despedida
	std::string partMsg;
	std::string restOfLine;
	std::getline(iss, restOfLine);

	if (!restOfLine.empty() && restOfLine[0] == ' ')
	{
		restOfLine = restOfLine.substr(1);
		if (!restOfLine.empty() && restOfLine[0] == ':')
			partMsg = restOfLine.substr(1);
	}

	// Crear mensaje del broadcast
	std::string broadcastMsg = ":" + cli->getNick() + "!~" + cli->getUser() + "@localhost PART " + channelName;

	if (!partMsg.empty())
		broadcastMsg += " :" + partMsg;

	broadcastMsg += "\r\n";
	ch->broadcast(broadcastMsg);
	ch->removeClient(cli);
	removeChannel(channelName);

	std::cout << cli->getNick() << " left " << channelName << std::endl;
}