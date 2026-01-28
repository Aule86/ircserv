#ifndef AUNTH_HPP
#define AUNTH_HPP

#include "Client.hpp"
#include <string>

class Aunth
{
public:
	static bool isAuthenticated(const Client &client);
	static bool canExecuteCommand(const Client &client, const std::string &command);
	static std::string getAuthError(const Client &client);
};

#endif