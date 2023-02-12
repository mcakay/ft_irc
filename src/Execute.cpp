#include "Server.hpp"
#include "Execute.hpp"

Execute::Execute(){
    this->commands.push_back(Command("JOIN", &Execute::join));
    this->commands.push_back(Command("PART", &Execute::part));
    this->commands.push_back(Command("PRIVMSG", &Execute::privmsg));
    this->commands.push_back(Command("QUIT", &Execute::quit));
    this->commands.push_back(Command("NICK", &Execute::nick));
    this->commands.push_back(Command("PING", &Execute::ping));
    this->commands.push_back(Command("KICK", &Execute::kick));
	this->commands.push_back(Command("PASS", &Execute::pass));
	this->commands.push_back(Command("USER", &Execute::user));
	//this->commands.push_back(Command("WHO", &Execute::who));
	//this->commands.push_back(Command("NOTICE", &Execute::notice));
	//this->commands.push_back(Command("MODE", &Execute::mode));
}

void Execute::join(int &fd, Server *server, std::string message){

	Channel *channel = server->getChannel(message);
	User *user = server->getUser(fd);
	size_t i = 0;
	
	if (channel == NULL){
		channel = new Channel(message);
		server->addChannel(channel);
		channel->addUser(user);
		channel->setOwner(user);
		channel->addOperator(user);
		server->sender(fd, utils::getProtocol(user) + " JOIN " +message);
		server->sender (fd, utils::getProtocol(user) + " MODE " + message + " +o " + user->getNickname());
		return;
	}

	if (channel->getUser(user) == NULL)
	{
		channel->addUser(user);

	}
	std::vector<User*> users = channel->getUsers();
	// for (std::vector<User*>::iterator it = users.begin(); it != users.end(); it++)
	// {
	// 	int fd2 = (*it)->getFd();
	// 	User *user2 = server->getUser(fd2);
	// 	server->sender(fd, utils::getProtocol(user2) + " JOIN " + message);
	// }
	for (std::vector<User*>::iterator it = users.begin(); it != users.end(); it++)
	{
		fd = (*it)->getFd();
		server->sender(fd, utils::getProtocol(user) + " JOIN " + message);
	}
	std::string usersList = ":ircserv 353 " + user->getNickname() + " = " + message + " :";
	for (std::vector<User*>::iterator it = users.begin(); it != users.end(); it++)
	{
		if (i == users.size() - 1)
			usersList += (*it)->getNickname();
		else
			usersList += (*it)->getNickname() + " ";
		i++;
	}
	usersList += "\r\n";
	usersList += ":ircserv 366 " + user->getNickname() + " " + message + " :End of /NAMES list."; 
	server->sender(fd, usersList);
}

void Execute::part(int &fd, Server *server, std::string message){
	(void)message;
	(void)server;
	(void)fd;
}

void Execute::privmsg(int &fd, Server *server, std::string message)
{
	std::string toWho = message.substr(0, message.find(" "));
	std::string msg = message.substr(message.find(" ") + 1);

	if (toWho[0] == '#')
	{
		
		std::string channelName = toWho;
		Channel *channel = server->getChannel(channelName);
		if (channel == NULL)
		{
			server->sender(fd, "ERROR :No such channel");
			return ;
		}
		std::vector<User*> users = channel->getUsers();
		for (std::vector<User*>::iterator it = users.begin(); it != users.end(); it++)
		{
			int fd2 = (*it)->getFd();
			User *user2 = server->getUser(fd2);
			server->sender(fd2, utils::getProtocol(user2) + " PRIVMSG " + channelName + " :" + msg);
		}
	}
}

void Execute::nick(int &fd, Server *server, std::string message){
	User *user = server->getUser(fd);
	if (user->getAuth() == true || server->getUser(message) != NULL)
	{
			server->sender(fd, utils::getProtocol(user) + " NICK " + message);
			user->setNickname(message);
	}
	user->setNickname(message);
}

void Execute::pass(int &fd, Server *server, std::string message){
	User *user = server->getUser(fd);
	if (message[0] != ':')
	{
		server->sender(fd, "ERROR :Not enough parameters");
		return ;		
	}
	auth::authPassword(user, server, message);
}

void Execute::kick(int &fd, Server *server, std::string message)
{
	(void)message;
	(void)server;
	(void)fd;
}

void Execute::quit(int &fd, Server *server, std::string message){
	(void)message;
	(void)server;
	(void)fd;
}

void Execute::ping(int &fd, Server *server, std::string message){
	(void)message;
	(void)server;
	(void)fd;
}

void Execute::user(int &fd, Server *server, std::string message){
	message = message.substr(0, message.find(" "));
	User *user = server->getUser(fd);
	user->setUsername(message);
}

void Execute::who(int &fd, Server *server, std::string message){

	(void)message;
	(void)server;
	(void)fd;
	 
}

void Execute::notice(int &fd, Server *server, std::string message){
	(void)message;
	(void)server;
	(void)fd;
}

void Execute::mode(int &fd, Server *server, std::string message){
	(void)message;
	(void)server;
	(void)fd;
}

std::string Execute::getCmd(std::string command){
    return command.substr(0, command.find(" "));
}

void Execute::execute(int &fd, Server *server, std::string message){
	std::string command = getCmd(message);
	User *user = server->getUser(fd);
	if (auth::checkAuth(user, server, command) == false)
		return;
	message = message.substr(message.find(" ") + 1);
	for (std::vector<Command>::iterator it = this->commands.begin(); it != this->commands.end(); it++){
		if (it->first == command){
			it->second(fd, server, message);
			return;
		}
	}
}

Execute::~Execute(){}