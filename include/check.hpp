#ifndef CHECK_HPP
# define CHECK_HPP

#include <iostream>
#include "Server.hpp"

#define USAGEERROR 1
#define PORTERROR 2
#define PORTNUMBERERROR 3
#define SOCKETERROR 4
#define BINDERROR 5
#define LISTENERROR 6
#define FCNTLERROR 7
#define SETSOCKOPTERROR 8 
#define POLLERROR 9
#define ACCEPTERROR 10
#define RECVERROR 11

class Server;

namespace check
{
	void	error(std::string msg, int exitCode);
	void	checkSocket(int sock, std::string function, int exitCode);
	void	checkArgs(int argc, char *argv[]);
	ssize_t checkRecv(int fd, char *buffer, int size);
}

#endif