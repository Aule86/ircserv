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
    sockaddr_in client_addr; // aqui iria la estructura de Client y esa movida acepta iP mas puerto IPv4
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_shocket, (sockaddr *)&client_addr, &client_len);
    
    if (client_fd == -1)
        return;
    fcntl(client_fd, F_SETFL, O_NONBLOCK);

    //seteando la estructura de pollfd para el nuevo cliente
    pollfd client_pollfd;
    client_pollfd.fd = client_fd;
    client_pollfd.events = POLLIN;
    client_pollfd.revents = 0;

    fds.push_back(client_pollfd);

    std::cout << "New connection accepted, fd: " << client_fd << std::endl;
}
void Server::data()
{
    std::cout << "Data received from client" << std::endl;
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
                    data();
                }
            }
            fds[i].revents = 0; // limpiar los eventos
        }
    }

    ServerClose();

    

    // Aquí iría la lógica para iniciar el servidor, aceptar conexiones, etc.
}

