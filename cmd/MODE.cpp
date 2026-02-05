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
					std::string currentParam = (paramIndex < params.size()) ? params[paramIndex++] : "";
					if (currentParam.empty())
						continue;
					std::cout << "[TODO] +k: Establecer password del canal: " << currentParam << std::endl;
				}
				else
					std::cout << "[TODO] -k: Quitar password del canal" << std::endl;
			}
			else if (flag == 'o')
			{
				if (adding)
				{
					std::string currentParam = (paramIndex < params.size()) ? params[paramIndex++] : "";
					if (currentParam.empty())
						continue;
					std::cout << "[TODO] +o: Dar privilegios de operador al usuario: " << currentParam << std::endl;
				}
				else
				{
					std::string currentParam = (paramIndex < params.size()) ? params[paramIndex++] : "";
					if (currentParam.empty())
						continue;
					std::cout << "[TODO] -o: Quitar privilegios de operador al usuario: " << currentParam << std::endl;
				}
			}
			else if (flag == 'l')
			{
				if (adding)
				{
					std::string currentParam = (paramIndex < params.size()) ? params[paramIndex++] : "";
					if (currentParam.empty())
						continue;
					std::cout << "[TODO] +l: Establecer límite de usuarios en el canal: " << currentParam << std::endl;
				}
				else
					std::cout << "[TODO] -l: Quitar límite de usuarios del canal" << std::endl;
			}
			else
			{
				std::string err = ":server 472 " + cli->getNick() + " " + flag + " :is unknown mode char to me\r\n";
				send(cli->getFd(), err.c_str(), err.length(), 0);
			}
	}
	
	// Respuesta temporal - modo sin cambios
	std::string msg = ":server 324 " + cli->getNick() + " " + target + " +\r\n";
	send(cli->getFd(), msg.c_str(), msg.length(), 0);
}