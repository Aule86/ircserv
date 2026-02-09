#include "../includes/Aunth.hpp"

bool Aunth::isAuthenticated(const Client &client)
{
	return client.hasPass() && 
		client.hasNick() && 
		client.hasUser();
}

bool Aunth::canExecuteCommand(const Client &client, const std::string &command)
{
	if (command == "QUIT")
		return true;

	// PASS solo si no se ha establecido todavía
	if (command == "PASS")
		return true;

	// NICK solo si ya se ha establecido PASS
	if (command == "NICK")
		return client.hasPass();

	// USER solo si ya se han establecido PASS y NICK
	if (command == "USER")
		return client.hasPass() && client.hasNick();

	// Otros comandos solo si está completamente autenticado
	return isAuthenticated(client);
}

std::string Aunth::getAuthError(const Client &client)
{
	if (!client.hasPass())
		return "ERROR :You must provide a password first (PASS)\r\n";

	if (!client.hasNick())
		return "ERROR :You must provide a nickname first (NICK)\r\n";

	if (!client.hasUser())
		return "ERROR :You must provide a username first (USER)\r\n";

	return "ERROR :Not authenticated\r\n";
}