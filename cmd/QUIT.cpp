#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Errors.hpp"
#include "../includes/Channel.hpp"

static std::string buildQuitMsg(Client *client, const std::string &message)
{
	if (message.empty())
		return ":" + client->getNick() + " QUIT :Client Quit\r\n";
	else
		return ":" + client->getNick() + " QUIT :" + message + "\r\n";
}

static void quitFromChannel(Channel *channel, Client *client, const std::string &quitMsg)
{
	if (!channel || !channel->hasClient(client))
		return ;

	channel->broadcast(quitMsg, client);
	channel->removeClient(client);
}

static void removeClientFromChannels(Server *server, Client *client, const std::string &quitMsg)
{
	std::vector<Channel*> channels = server->getAllChannels();
	size_t i;
	for (i = 0; i < channels.size(); ++i)
	{
		quitFromChannel(channels[i], client, quitMsg);
	}
}

void Server::handleQUIT(Client *cli, std::istringstream &iss)
{
    std::string message;

    if (iss.peek() == ' ')
        iss.get();
    std::getline(iss, message);
    if (!message.empty() && message[0] == ':')
        message = message.substr(1);

    std::string quitMsg = buildQuitMsg(cli, message);

    removeClientFromChannels(this, cli, quitMsg);
    cli->sendMessage(quitMsg);

    int fd = cli->getFd(); // ← guardamos el fd ANTES de destruir cli
    removeClient(fd);      // ← aquí se hace delete cli, el puntero queda inválido

    close(fd);             // ← usamos la variable local, no cli->getFd()
    std::cout << "[CLIENT " << fd << "] QUIT: " << message << std::endl;
}