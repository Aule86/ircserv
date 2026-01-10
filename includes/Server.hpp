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



#define RED "\e[1;31m"

class Server
{
    private:
            int port;
            std::string password;
            static bool signal;
            int server_shocket;
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
};

#endif
