#pragma once

#include <fcntl.h>
#include "data.hpp"
#include <map>
#include "user.hpp"

class serv : public Data
{
	map<int, User> users;
	// map<string, User> users;
	map<string, void(serv::*)(string, User&)> cmd;

	bool	checkSockFD();
	int		maxFD();
	bool	checkCommand(char *buf);
	bool	read_write(int fd);
	int		findUserByNick(string nick);
	void	joinChannel(User &user, Channel &chan, std::vector<string> arr, bool flag);
	bool	checkChannelNameKey(std::vector<std::string> arr, bool flag);
	bool	joinWithTwoArgs(User &user, Channel &chan, std::vector<string> arr, bool flag);
	bool	joinWithOneArgs(User &user, Channel &chan, std::vector<string> arr, bool flag);
	void	sendAll(std::map<int, User> use, std::string cmd, std::string msg);
public:
	serv();
////////////////////---COMMANDS---//////////////////////
	void	user(string b, User &user);
	void	pass(string b, User &user);
	void	nick(string b, User &user);
	void	ping(string b, User &user);
	void	pong(string b, User &user);
	void	quit(string b, User &user);
	void	privmsg(string b, User &user);
	// void	error(string b, User &new_user);
	
	void	ls(string b, User &user);
	void	cap(string b, User &user);
	void	join(string b, User &user);
	void	kick(string b, User &user);
	void	invite(string b, User &user);
	void	topic(string b, User &user);
	// void	mode(string b, User &user);
//////////////////////////////////////////////////////
///////////////////////HELP///////////////////////////
	bool	startServ();
	void	add_client();

};

