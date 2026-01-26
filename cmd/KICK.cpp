#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Errors.hpp"
#include "../includes/Channel.hpp"
#include <sstream>

static bool parseKick(std::istringstream &iss, std::string &channel, std::string &target, std::string &reason)
{
	iss >> channel >> target;
	std::getline(iss, reason);

	if (!reason.empty() && reason[0] == ' ')
		reason.erase(0,1);
	if (!reason.empty() && reason[0] == ':')
		reason.erase(0,1);
	return !(channel.empty() || target.empty());
}

static Channel *validateKick(Server *server, Client *cli, const std::string &channelName, const std::string &target, Client *targetClient)
{
	Channel *chan = server->getChannel(channelName);
	if (!chan)
	{
		server->sendError(cli, ERR_NOSUCHCHANNEL, channelName);
		return (NULL);
	}
	if (!chan->hasClient(cli))
	{
		server->sendError(cli, ERR_NOTONCHANNEL, channelName);
		return (NULL);
	}
	if (!chan->isOperator(cli))
	{
		server->sendError(cli, ERR_CHANOPRIVSNEEDED, channelName);
		return (NULL);
	}
	targetClient = server->getClientByName(target);
	if (!targetClient)
	{
		server->sendError(cli, ERR_NOSUCHNICK, target);
		return (NULL);
	}
	if (!chan->hasClient(targetClient))
	{
		server->sendError(cli, ERR_USERNOTINCHANNEL, target + " " + channelName);
		return (NULL);
	}
	return chan;
}

static void executeKick(Channel *chan, Client *cli, Client *target, const std::string &channelName, const std::string &reason)
{
	std::string msg = ":" + cli->getPrefix() + "KICK" + channelName + " " + target->getNick() + ":" + reason + "\r\n";
	chan->broadcast(msg);
	chan->removeClient(target);
}

void Server::handleKICK(Server *server, Client *cli, std::istringstream &iss)
{
	std::string channelName;
	std::string target;
	std::string reason;
	
	Client *targetClient = NULL;

	if (!parseKick (iss, channelName, target, reason))
	{
		server->sendError(cli, ERR_NEEDMOREPARAMS, "KICK: Not enough parameters");
		return;
	}
	Channel *chan = validateKick(server, cli, channelName, target, targetClient);
	if (!chan)
		return;
	
	if (reason.empty())
		reason = cli->getNick();
	executeKick(chan, cli, targetClient, channelName, reason);

}