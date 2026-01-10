
#include "includes/Server.hpp"


bool ValidPort(const std::string &port)
{
    if (port.empty())
        return false;

    if (port.find_first_not_of("0123456789") != std::string::npos)
        return false;

    int p = std::atoi(port.c_str());
    if (p < 1024 || p > 65535)
        return false;

    return true;
}


int main(int argc, char **argv)
{
	Server server;

    if(argc != 3)
    {
        std::cout << RED << "INVALID" << std::endl;
        std::cout << "Correct way: " << argv[0] << " <port number> " << "<password>" << std::endl;
        return(0);
    }
    try
    {
        signal(SIGINT, Server::SignalResponse);
		signal(SIGQUIT, Server::SignalResponse);// hay que hacer el gestor de señales para cerrar todos los sockets
		signal(SIGPIPE, SIG_IGN);

        if(!ValidPort(argv[1]) || !*argv[2])
        {
			std::cout << RED << "INVALID" << std::endl;
			std::cout << "Wrong port " << "Wrong password" << std::endl;
			return 1;
        }
		server.start(std::atoi(argv[1]), argv[2]);

    }
	catch(const std::exception& e)
	{

	}
	return(0);

}