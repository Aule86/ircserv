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

	if (!ch)
		ch = createChannel(channelName, cli);

	ch->addClient(cli);

	// Notificar al cliente que hizo JOIN
	std::string joinMsg = ":" + cli->getPrefix() + " JOIN " + channelName + "\r\n";
	ch->broadcast(joinMsg);

	std::cout << "Client " << cli->getNick() << " joined " << channelName << std::endl;
}