#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Errors.hpp"
#include "../includes/Channel.hpp"
#include <sstream>

/*
 * MODE - Comando para establecer/obtener modos de canal
 * 
 * Esta es una implementación básica que solo muestra los modos actuales.
 * TODO: Implementar cambios de modos (+i, +t, +k, +o, +l)
 */

void Server::handleMODE(Client *cli, std::istringstream &iss)
{
	std::string target;
	std::string modestring;
	
	iss >> target >> modestring;
	
	// Leer todos los parámetros restantes
	std::vector<std::string> params;
	std::string param;
	while (iss >> param)
		params.push_back(param);
	
	// Validación básica
	if (target.empty())
	{
		std::string err = ":server 461 " + cli->getNick() + " MODE :Not enough parameters\r\n";
		send(cli->getFd(), err.c_str(), err.length(), 0);
		return;
	}
	
	// Por ahora solo soportamos MODE para canales (empiezan con #)
	if (target[0] != '#')
	{
		std::string msg = ":server 501 " + cli->getNick() + " :Unknown MODE flag\r\n";
		send(cli->getFd(), msg.c_str(), msg.length(), 0);
		return;
	}
	
	// Buscar el canal
	Channel *channel = this->getChannel(target);
	if (!channel)
	{
		std::string err = ":server 403 " + cli->getNick() + " " + target + " :No such channel\r\n";
		send(cli->getFd(), err.c_str(), err.length(), 0);
		return;
	}
	
	// Verificar que el cliente está en el canal
	if (!channel->hasClient(cli))
	{
		std::string err = ":server 442 " + cli->getNick() + " " + target + " :You're not on that channel\r\n";
		send(cli->getFd(), err.c_str(), err.length(), 0);
		return;
	}
	
	// Si no se especifica modo, solo mostrar los modos actuales
	if (modestring.empty())
	{
		std::string msg = ":server 324 " + cli->getNick() + " " + target + " +\r\n";
		send(cli->getFd(), msg.c_str(), msg.length(), 0);
		return;
	}
	
	// Verificar que el cliente es operador para cambiar modos
	if (!channel->isOperator(cli))
	{
		std::string err = ":server 482 " + cli->getNick() + " " + target + " :You're not channel operator\r\n";
		send(cli->getFd(), err.c_str(), err.length(), 0);
		return;
	}
	
	// TODO: Implementar los siguientes modos
	// Parsear el modestring (ejemplo: +i, -t, +k password, +o nick, +l 10)
	
	// Procesar cada flag en el modestring
	bool adding = true;
	size_t paramIndex = 0; // Índice para recorrer los parámetros
	for (size_t i = 0; i < modestring.length(); ++i)
	{
		char flag = modestring[i];
		
		if (flag == '+')
		{
			adding = true;
			continue;
		}
		else if (flag == '-')
		{
			adding = false;
			continue;
		}
		
		// Procesar cada modo
		// Procesar cada modo
		if (flag == 'i')
		{
			if (adding)
			{
				// +i: Establecer canal como invite-only
				channel->setInviteOnly(true);
				std::cout << cli->getNick() << " set +i on " << target << std::endl;
				
				// Notificar a todos en el canal
				std::string modeMsg = ":" + cli->getNick() + "!~" + cli->getUser() + cli->getPrefix() + target + " +i\r\n";
				channel->broadcast(modeMsg);
			}
			else
			{
				// -i: Quitar modo invite-only
				channel->setInviteOnly(false);
				std::cout << cli->getNick() << " set -i on " << target << std::endl;
				
				// Notificar a todos en el canal
				std::string modeMsg = ":" + cli->getNick() + "!~" + cli->getUser() + cli->getPrefix() + target + " -i\r\n";
				channel->broadcast(modeMsg);
			}
		}
			else if (flag == 't')
			{
				std::cout <<" entra en topic "<< std::endl;
				if (adding)
				{
					channel->setTopicRestricted(true);
					std::cout << cli->getNick() << " set +t on " << target << std::endl;
					std::string modeMsg =":" + cli->getPrefix() + " MODE " + target + " +t\r\n";

					channel->broadcast(modeMsg);

				}

				else
				{
					channel->setTopicRestricted(false);
					std::cout << cli->getNick() << " set -t on " << target << std::endl;
					std::string modeMsg =":" + cli->getPrefix() + " MODE " + target + " -t\r\n";

					channel->broadcast(modeMsg);
				}
			}
			else if (flag == 'k')
			{
				if (adding)
				{
					std::string key;
					if (paramIndex < params.size())
						key = params[paramIndex++];
					else
						key = "";
					if (key.empty())
					{
						std::string err = ":server 461 " + cli->getNick() + " MODE :Not enough parameters\r\n";
						send(cli->getFd(), err.c_str(), err.length(), 0);
						return;
					}
					channel->setKey(key);
					std::cout << cli->getNick() << " set +k on " << target << std::endl;
					std::string modeMsg = ":" + cli->getPrefix() + " MODE " + target + " +k \r\n";
					channel->broadcast(modeMsg);
				}
				else
				{
					channel->setKeyActive(false);
					std::cout << cli->getNick() << " set -k on " << target << std::endl;
					std::string modeMsg = ":" + cli->getPrefix() + " MODE " + target + " -k\r\n";
					channel->broadcast(modeMsg);
				}
			}
			else if (flag == 'o')
			{
				std::string opNick;
				if (paramIndex < params.size())
					opNick = params[paramIndex++];
				else
					opNick = "";
				if (opNick.empty())
					continue;
				
				Client *opClient = channel->getClientByName(opNick);
				if (!opClient)
					continue;
				if (adding)
				{
					channel->addOperator(opClient);
					std::string modeMsg = ":" + cli->getPrefix() + " MODE " + target + " +o " + opNick + "\r\n";
					channel->broadcast(modeMsg);
				}
				else
				{
					channel->removeOperator(opClient);
					std::string modeMsg = ":" + cli->getPrefix() + " MODE " + target + " -o " + opNick + "\r\n";
					channel->broadcast(modeMsg);
				}
			}
			else if (flag == 'l')
			{
				if (adding)
				{
					std::string limitStr;
					if (paramIndex < params.size())
						limitStr = params[paramIndex++];
					else
						continue;
					int limit = std::stoi(limitStr);
					channel->setUserLimit(limit);
					std::string modeMsg = ":" + cli->getPrefix() + " MODE " + target + " +l " + limitStr + "\r\n";
					channel->broadcast(modeMsg);
				}
				else
				{
					channel->removeUserLimit();
					std::string modeMsg = ":" + cli->getPrefix() + " MODE " + target + " -l\r\n";
					channel->broadcast(modeMsg);
				}
			}
	}
	
	// Respuesta temporal - modo sin cambios
	std::string msg = ":server 324 " + cli->getNick() + " " + target + " +\r\n";
	send(cli->getFd(), msg.c_str(), msg.length(), 0);
}