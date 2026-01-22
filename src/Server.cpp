#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Aunth.hpp"

Server::Server(){}

Server::~Server(){}

void Server::handlePASS(Client *cli, std::istringstream &iss)
{
	std::string pass;
	iss >> pass;

	if (pass.empty())
	{
		std::string msg = "ERROR :Password is empty\r\n";
		send(cli->getFd(), msg.c_str(), msg.length(), 0);
		return;
	}

	if (pass == password)
		cli->setHasPass(true);
	else
	{
		std::string msg = "ERROR :Password incorrect\r\n";
		send(cli->getFd(), msg.c_str(), msg.length(), 0);
		std::cout << "Client " << cli->getFd() << " - Password incorrect" << std::endl;
	}
}

void Server::handleNICK(Client *cli, std::istringstream &iss)
{
	std::string nick;
	iss >> nick;
	
	if (nick.empty())
	{
		std::string msg = "ERROR :Nickname is empty\r\n";
		send(cli->getFd(), msg.c_str(), msg.length(), 0);
		return;
	}
	
	cli->setNick(nick);
}

void Server::handleUSER(Client *cli, std::istringstream &iss)
{
	std::string user;
	iss >> user;
	
	if (user.empty())
	{
		std::string msg = "ERROR :Username is empty\r\n";
		send(cli->getFd(), msg.c_str(), msg.length(), 0);
		return;
	}
	
	cli->setUser(user);
}

void Server::handleCommand(Client *cli, const std::string &lines)
{
	std::istringstream iss(lines);
	std::string cmd;
	iss >> cmd;

	if (!Aunth::canExecuteCommand(*cli, cmd))
	{
		std::string error = Aunth::getAuthError(*cli);
		send(cli->getFd(), error.c_str(), error.length(), 0);
		return;
	}

	if (cmd == "PASS")
		handlePASS(cli, iss);
	else if (cmd == "NICK")
		handleNICK(cli, iss);
	else if (cmd == "USER")
		handleUSER(cli, iss);

	tryRegister(*cli);
}

void Server::tryRegister(Client &cli)
{
	if (cli.isRegistered())
		return;
	if (Aunth::isAuthenticated(cli))
	{
		cli.setRegistered(true);

		std::string msg = ":ircserv 001 " + cli.getNick() + " :Welcome to the IRC server\r\n";

		send(cli.getFd(), msg.c_str(), msg.length(), 0);
		std::cout << "Client " << cli.getFd() << " (" << cli.getNick() << ") successfully registered" << std::endl;
	}
}

bool Server::signal = false;

void Server::setPort(int port)
{
	this->port = port;
}

void Server::setPassword(std::string password)
{
	this->password = password;
}
int Server::getPort()
{
	return this->port;
}
std::string Server::getPassword()
{
	return this->password;
}
Client* Server::getClient(int fd)
{
	std::map<int, Client*>::iterator it = clients.find(fd);
	if (it == clients.end())
		return NULL;
	return it->second;
}


void Server::ServerClose()
{
	for (size_t i = 0; i < fds.size(); ++i)
	{
		close(fds[i].fd);
	}
	fds.clear();
}

void Server::SignalResponse(int signum)
{
	if (signum == SIGINT || signum == SIGQUIT)
	{
		std::cout << "\nSignal received, shutting down server..." << std::endl;
		Server::signal = true;
	}
}
void Server::setupServerSocket()
{
	server_shocket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_shocket == -1)
		throw std::runtime_error("socket() failed");

	int opt = 1;
	if (setsockopt(server_shocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("setsockopt() failed");

	sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(this->getPort());

	if (bind(server_shocket, (sockaddr *)&addr, sizeof(addr)) < 0)
		throw std::runtime_error("bind() failed");

	if (listen(server_shocket, SOMAXCONN) < 0)
		throw std::runtime_error("listen() failed");

	fcntl(server_shocket, F_SETFL, O_NONBLOCK);
}
void Server::new_conection()
{
	sockaddr_in cliadd;
	socklen_t len = sizeof(cliadd);
	memset(&cliadd, 0, sizeof(cliadd));

	int fd = accept(server_shocket, (sockaddr *)&cliadd, &len);
	if (fd == -1)
		return;

	fcntl(fd, F_SETFL, O_NONBLOCK);


	new_cli.fd = fd;
	new_cli.events = POLLIN;
	new_cli.revents = 0;
	fds.push_back(new_cli);

	Client *cli = new Client(fd);

	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &cliadd.sin_addr, ip, INET_ADDRSTRLEN);
	cli->setIpAdd(ip);

	clients[fd] = cli;

	std::cout << "Client <" << fd << "> Connected from " << ip << std::endl;

}


void Server::receiveData(int fd)
{
	char	tmp[512];
	memset(tmp, 0, sizeof(tmp));
	Client *cli = getClient(fd);
	ssize_t	bytes = recv(fd, tmp, sizeof(tmp) - 1, 0);

	if (bytes <= 0)
	{
		std::cout << RED << "Client with fd " << fd << " disconnected." << std::endl;
		return;

	}else
	{
		cli->setBuffer(std::string(tmp, bytes));
		std::vector<std::string> lines = cli->extractLines();
		for (size_t i = 0; i < lines.size(); i++)
		{
			handleCommand(cli, lines[i]);
			std::cout << "[CLIENT " << fd << "] " << lines[i] << std::endl;
		}
	}
	return;
}

void Server::start(int port, std::string password)
{
	this->setPort(port);
	this->setPassword(password);    

	setupServerSocket();

	pollfd server_pollfd;
	server_pollfd.fd = server_shocket;
	server_pollfd.events = POLLIN;
	server_pollfd.revents = 0;
	fds.push_back(server_pollfd);

	std::cout << "Server started on port: " << this->getPort() << " with password: " << this->getPassword() << std::endl;

	while(Server::signal == false)
	{
		if(poll(fds.data(), fds.size(), -1) == -1)
		{
			std::cerr << "poll() failed" << std::endl;
			break;
		}
		for(size_t i = 0; i < fds.size(); i++)
		{
			if(fds[i].revents & POLLIN)
			{
				if(fds[i].fd == server_shocket)
				{
					std::cout   << "New connection incoming..." << std::endl;
					new_conection();
				}
				else
				{
					receiveData(fds[i].fd);
				}
			}
			fds[i].revents = 0;
		}
	}

	ServerClose();
}