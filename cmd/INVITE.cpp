#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Errors.hpp"
#include "../includes/Channel.hpp"
#include <sstream>

// Busca un cliente por su nickname
Client* Server::getClientByNick(const std::string &nick)
{
    // Itera sobre todos los clientes
    for (std::map<int, Client*>::iterator it = clients.begin();
         it != clients.end();
         ++it)
    {
        // Compara el nickname
        if (it->second->getNick() == nick)
            return it->second;
    }
    return (NULL);
}

void Server::handleINVITE(Client *cli, std::istringstream &iss)
{
	std::string targetNick;
	std::string channelName;

	iss >> targetNick >> channelName;

	if (targetNick.empty() || channelName.empty())
	{
		std::string err = ":server 461 " + cli->getNick() + " INVITE :Not enough parameters\r\n";
		send(cli->getFd(), err.c_str(), err.length(), 0);
		return;
	}

	// Validación: el canal debe empezar con #
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

	// Verificar que quien invita está en el canal
	if (!ch->hasClient(cli))
	{
		std::string err = ":server 442 " + cli->getNick() + " " + channelName + " :You're not on that channel\r\n";
		send(cli->getFd(), err.c_str(), err.length(), 0);
		return;
	}

	// Verificar que quien invita está en el canal
	if (!ch->hasClient(cli))
	{
		std::string err = ":server 442 " + cli->getNick() + " " + channelName + " :You're not on that channel\r\n";
		send(cli->getFd(), err.c_str(), err.length(), 0);
		return;
	}

	// Verificar que quien invita es operador (opcional, depende de tu implementación)
	if (!ch->isOperator(cli))
	{
		std::string err = ":server 482 " + cli->getNick() + " " + channelName + " :You're not channel operator\r\n";
		send(cli->getFd(), err.c_str(), err.length(), 0);
		return;
	}

	// Buscar al usuario a invitar
	Client *target = getClientByNick(targetNick);
	if (!target)
	{
		std::string err = ":server 401 " + cli->getNick() + " " + targetNick + " :No such nick\r\n";
		send(cli->getFd(), err.c_str(), err.length(), 0);
		return;
	}

	// Verificar que el usuario invitado no está ya en el canal
	if (ch->hasClient(target))
	{	
		std::string err = ":server 443 " + cli->getNick() + " " + targetNick + " " + channelName + " :is already on channel\r\n";
		send(cli->getFd(), err.c_str(), err.length(), 0);
		return;
	}

	// Todo OK - Enviar invitación al usuario target
	std::string inviteMsg = ":" + cli->getNick() + "!~" + cli->getUser() + cli->getPrefix() + targetNick + " " + channelName + "\r\n";
	target->sendMessage(inviteMsg);

	// Confirmar al que invitó
	std::string confirmMsg = ":server 341 " + cli->getNick() + " " + targetNick + " " + channelName + "\r\n";
	cli->sendMessage(confirmMsg);
	
	ch->addInvited(target); 

	std::cout << cli->getNick() << " invited " << targetNick << " to " << channelName << std::endl;
}