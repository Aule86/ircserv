
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
#include <stdlib.h>

#define RED "\e[1;31m"
#define WHI "\e[0;37m"

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
		Client *getClientByName(const std::string &nick) const;
		void removeClient(int fd);

		//Setters
		void setPort(int port);
		void setPassword(std::string password);

		static void SignalResponse(int signum);
		void start(int port, std::string password);
		void setupServerSocket();
		void ServerClose();
		void new_conection();
		void receiveData(int fd);
		void removePollFd(int fd);

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
		std::vector<Channel*> getAllChannels() const;


		//cmd
		void handleJOIN(Client *cli, std::istringstream &iss);
		void handleKICK(Server *server, Client *cli, std::istringstream &iss);
		void handlePRIVMSG(Server *server, Client *cli, std::istringstream &iss);
		void handleQUIT(Client *cli, std::istringstream &iss);
		

		//errores y replies
		void sendError(Client *cli, const std::string &code, const std::string &msg);
		void sendReply(Client *cli, const std::string &code, const std::string &msg);

		//invite

		void handleINVITE(Client *cli, std::istringstream &iss);
		Client* getClientByNick(const std::string &nick);

};

#endif
