#include "../includes/Server.hpp"

Server::Server(){}

Server::~Server(){}



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


void Server::start(int port, std::string password)
{
    this->setPort(port);
    this->setPassword(password);

    std::cout << "Server started on port: " << this->getPort() << " with password: " << this->getPassword() << std::endl;

    // Aquí iría la lógica para iniciar el servidor, aceptar conexiones, etc.
}

