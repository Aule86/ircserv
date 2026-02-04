#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Errors.hpp"
#include "../includes/Channel.hpp"


/* void Server::handleTOPIC(Client *cli, std::istringstream &iss)
{
    std::string channelName;
    iss >> channelName;

    if (channelName.empty())
    {
        sendError(cli, ERR_NEEDMOREPARAMS, "TOPIC :Not enough parameters");
        return;
    }

    Channel *channel = getChannel(channelName);
    if (!channel)
    {
        sendError(cli, ERR_NOSUCHCHANNEL, channelName + " :No such channel");
        return;
    }

    if (!channel->hasClient(cli))
    {
        sendError(cli, ERR_NOTONCHANNEL, channelName + " :You're not on that channel");
        return;
    }

    std::string topic;
    std::getline(iss, topic);
    if (!topic.empty() && topic[0] == ' ')
        topic.erase(0, 1); // Remove leading space

    if (topic.empty())
    {
        // Get current topic
        if (channel->getTopic().empty())
        {
            sendReply(cli, "331", channelName + " :No topic is set");
        }
        else
        {
            sendReply(cli, "332", channelName + " :" + channel->getTopic());
        }
    }
    else
    {
        // Set new topic
        if (!channel->isOperator(cli))
        {
            sendError(cli, ERR_CHANOPRIVSNEEDED, channelName + " :You're not a channel operator");
            return;
        }

        channel->setTopic(topic);
        std::string msg = ":" + cli->getNick() + " TOPIC " + channelName + " :" + topic + "\r\n";
        channel->broadcast(msg);
    }
} */

void Server::handleTOPIC(Client *cli, std::istringstream &iss)
{
    std::string channelName;
    iss >> channelName;

    if (channelName.empty())
    {
        sendError(cli, ERR_NEEDMOREPARAMS, "TOPIC :Not enough parameters");
        return;
    }

    Channel *channel = getChannel(channelName);
    if (!channel)
    {
        sendError(cli, ERR_NOSUCHCHANNEL, channelName); // error no hay ese canal cambiar mas visible
        return;
    }

    if (!channel->hasClient(cli))
    {
        sendError(cli, ERR_NOTONCHANNEL, channelName);
        return;
    }

    std::string topic;
    std::getline(iss, topic);

    if (!topic.empty() && topic[0] == ' ')
        topic.erase(0, 1);
    if (!topic.empty() && topic[0] == ':')
        topic.erase(0, 1);

    // 🔹 SOLO CONSULTA
    if (topic.empty())
    {
        if (channel->getTopic().empty())
            sendReply(cli, "331", channelName + " :No topic is set");
        else
        {
            sendReply(cli, "332", channelName + " " + channel->getTopic());
            std::cout << "TOPIC DEBUG: [" << channel->getTopic() << "]" << std::endl;


        }
        return;
    }

    // 🔹 CAMBIO DE TOPIC c par el mode channel->isTopicRestricted()
    if ( !channel->isOperator(cli))
    {
        sendError(cli, ERR_CHANOPRIVSNEEDED, channelName);
        return;
    }

    channel->setTopic(topic);

    std::string msg = ":" + cli->getPrefix()
        + " TOPIC " + channelName + " :" + topic + "\r\n";

    channel->broadcast(msg);
}


void Channel::setTopic(const std::string &topic)
{
    _topic = topic;
}

const std::string& Channel::getTopic() const
{
    return _topic;
}