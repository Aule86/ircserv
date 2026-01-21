#include "../includes/Aunth.hpp"

bool Aunth::isAuthenticated(const Client &client)
{
    return client.hasPass() && 
           !client.getNick().empty() && 
           !client.getUser().empty();
}

bool Aunth::canExecuteCommand(const Client &client, const std::string &command)
{
    if (command == "PASS" || command == "NICK" || 
        command == "USER" || command == "QUIT")
    {
        return true;
    }
    
    return isAuthenticated(client);
}

std::string Aunth::getAuthError(const Client &client)
{
    if (!client.hasPass())
        return "ERROR :You must provide a password first (PASS)\r\n";
    
    if (client.getNick().empty())
        return "ERROR :You must provide a nickname (NICK)\r\n";
    
    if (client.getUser().empty())
        return "ERROR :You must provide a username (USER)\r\n";
    
    return "ERROR :Not authenticated\r\n";
}