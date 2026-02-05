#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Errors.hpp"
#include "../includes/Channel.hpp"


bool Channel::isTopicRestricted() const
{
    return _topicRestricted;
}

void Channel::setTopicRestricted(bool value)
{
    _topicRestricted = value;
}

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
        sendError(cli, ERR_NOSUCHCHANNEL, channelName + " : NO SUCH CHANELL"); 
        return;
    }

    if (!channel->hasClient(cli))
    {
        sendError(cli, ERR_NOTONCHANNEL, channelName + " : NOT ON CHANELL");
        return;
    }

    std::string topic;
    std::getline(iss, topic);

    if (!topic.empty() && topic[0] == ' ')
        topic.erase(0, 1);
    if (!topic.empty() && topic[0] == ':')
        topic.erase(0, 1);


    if (topic.empty())
    {
        if (channel->getTopic().empty())
            sendReply(cli, "331", channelName + " :No topic is set");
        else
            sendReply(cli, "332", channelName + " :" + channel->getTopic());

        return;
    }


    // 🔹 CAMBIO DE TOPIC c par el mode channel->isTopicRestricted()
    if (channel->isTopicRestricted() && !channel->isOperator(cli))
    {
        sendError(cli, ERR_CHANOPRIVSNEEDED, channelName + " : NO OPERATOR PRIVILEGES");
        return;
    }

    channel->setTopic(topic);

    std::string msg = ":" + cli->getPrefix()
        + " TOPIC " + channelName + " :" + topic + " " +"\r\n";

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