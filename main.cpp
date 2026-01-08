#include "includes/Server.hpp"
#include "includes/Client.hpp"


int main(int ac, char **av) {
	if (ac != 2) {
		std::cerr << "Usage: ./ircserv <port>" << std::endl;
		return 1;
		}

		try {
		Server server(std::atoi(av[1]));
		server.run();
		} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}
