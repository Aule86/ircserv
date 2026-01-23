
#ifndef SERVER
#define SERVER

#include <iostream>
#include <sys/socket.h>
#include <poll.h>
#include <signal.h>
#include <vector>
#include <string>
#include <cstring>
#include <set>

//includes de sockets
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>


#include <map>
#include <unistd.h>

#define RED "\e[1;31m"

class Channel;
class Client;

class Server
{
	private:
		int port;
		std::string password;
		static bool signal;
		int server_shocket;
		std::vector<struct pollfd> fds;
		std::map<int, Client *> clients;
		std::map<std::string, Channel*> channels;
		struct pollfd new_cli;

		struct pollfd new_client;
	public:
		Server();
		~Server();

		//Getters
		int getPort();
		std::string getPassword();
		Client* getClient(int fd);

		//Setters

		void setPort(int port);
		void setPassword(std::string password);

		static void SignalResponse(int signum);
		void start(int port, std::string password);
		void setupServerSocket();
		void ServerClose();
		void new_conection();
		void receiveData(int fd);

		//irc commands
		void handleCommand(Client *cli, const std::string &line);
		void tryRegister(Client &cli);

		void handlePASS(Client *cli, std::istringstream &iss);
		void handleNICK(Client *cli, std::istringstream &iss);
		void handleUSER(Client *cli, std::istringstream &iss);

		//channel
		Channel* getChannel(const std::string &name);
		Channel* createChannel(const std::string &name, Client *creator);
		void removeChannel(const std::string &name);

		//cmd
		void handleJOIN(Client *cli, std::istringstream &iss);



};

#endif
