#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Errors.hpp"
#include "../includes/Channel.hpp"

#include <sstream>

static bool parsePrivmsg(std::istringstream &iss, std::string &target, std::string &message)
{
	iss >> target;
	std::getline(iss, message);

	if (!message.empty() && message[0] == ' ')
		message.erase(0, 1);
	if (!message.empty() && message[0] == ':')
		message.erase(0, 1);
	
	return (!target.empty());
}

static void sendPrivmsgToChannel(Server *server, Client *cli, const std::string &target, const std::string &message)
{
	Channel *chan = server->getChannel(target);
	if (!chan)
	{
		server->sendError(cli, ERR_NOSUCHCHANNEL, target);
		return ;
	}
	if (!chan->hasClient(cli))
	{
		server->sendError(cli, ERR_CANNOTSENDTOCHAN, target);
		return ;
	}

	std::string msg = ":" + cli->getPrefix() + " PRIVMSG " + target + " :" + message + "\r\n";
	chan->broadcast(msg, cli);
}

static void sendPrivmsgToNick(Server *server, Client *cli, const std::string &target, const std::string &message)
{
	Client *dest = server->getClientByName(target);
	if (!dest)
	{
		server->sendError(cli, ERR_NOSUCHNICK, target);
		return ;
	}
	
	std::string msg = ":" + cli->getPrefix() + " PRIVMSG " + target + " :" + message + "\r\n";
	dest->sendMessage(msg);
}

void Server::handlePRIVMSG(Server *server, Client *cli, std::istringstream &iss)
{
	std::string target;
	std::string message;

	if (!parsePrivmsg(iss, target, message))
	{
		server->sendError(cli, ERR_NORECIPIENT, "PRIVMSG : No recipient given");
		return ;
	}
	if (message.empty())
	{
		server->sendError(cli, ERR_NOTEXTTOSEND, ": No text to send");
		return ;
	}

	//canal o ususario
	if (target[0] == '#')
		sendPrivmsgToChannel(server, cli, target, message);
	else
		sendPrivmsgToNick(server, cli, target, message);
}