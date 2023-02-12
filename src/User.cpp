#include "User.hpp"

// Constructors and destructors

User::User(){}

User::~User(){}

User::User(int fd){
	this->fd = fd;
	this->auth = false;
}

// Getters and setters

std::string User::getUsername()
{
	return (this->username);
}


std::string User::getNickname()
{
	return (this->nickname);
}

void User::setUsername(std::string username)
{
	this->username = username;
}

void User::setNickname(std::string nickname)
{
	this->nickname = nickname;
}

void User::setFd(int fd)
{
	this->fd = fd;
}

int User::getFd()
{
	return (this->fd);
}

bool User::getAuth()
{
	return (this->auth);
}

void User::setAuth(bool auth)
{
	this->auth = auth;
}