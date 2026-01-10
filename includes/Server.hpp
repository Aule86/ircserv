/* #ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include <string>
#include <iostream>
#include <cstdlib>
#include <csignal>


class Server
{

}

#endif */

#ifndef SERVER
#define SERVER

#include <iostream>
#include <sys/socket.h>
#include <poll.h>
#include <signal.h>
#include <vector>
#include <string>
#include <cstring>


//includes de sockets
#include <netinet/in.h>
#include <fcntl.h>




#define RED "\e[1;31m"

class Server
{
    private:
            int port;
            std::string password;
            static bool signal;
            int server_shocket;
            std::vector<struct pollfd> fds;
    public:
            Server();
            ~Server();

            //Getters
            int getPort();
            std::string getPassword();

            //Setters

            void setPort(int port);
            void setPassword(std::string password);

            static void SignalResponse(int signum);
            void start(int port, std::string password);
            void setupServerSocket();
            void ServerClose();
            void new_conection();
            void data();
};

#endif
