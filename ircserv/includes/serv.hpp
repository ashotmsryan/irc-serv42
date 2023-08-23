#ifndef SERV_HPP
# define SERV_HPP

#include <fcntl.h>
#include <map>
#include "data.hpp"
#include "user.hpp"

class User;
class Channel;


class serv : public Data
{
	map<int, User> users;
	map<string, void(serv::*)(string, User&)> cmd;
	std::vector<int> fd;
	bool	checkSockFD();
	bool	checkNumber(std::string n);
	int		maxFD();
	bool	checkCommand(char *buf);
	bool	read_write(int fd);
	void	removeFromChannels(int fd);
	int		findUserByNick(string nick);
	void	joinChannel(User &user, Channel &chan, std::vector<string> arr, bool flag);
	bool	checkChannelNameKey(std::vector<std::string> arr);
	bool	joinWithTwoArgs(User &user, Channel &chan, std::vector<string> arr, bool flag);
	bool	joinWithOneArgs(User &user, Channel &chan, std::vector<string> arr, bool flag);
	void	sendAll(std::map<int, User&> use, std::string cmd, std::string msg);
	std::vector<std::string> parsing(std::string buf);

	map<std::string, Channel&>::iterator findChannelsFromUsers(std::string name);
public:
	serv();


	std::map<std::string, Channel> all_channels;
////////////////////---COMMANDS---//////////////////////
	void	user(string b, User &user);
	void	pass(string b, User &user);
	void	nick(string b, User &user);
	void	ping(string b, User &user);
	void	pong(string b, User &user);
	void	quit(string b, User &user);
	void	privmsg(string b, User &user);
	void	notice(string b, User &new_user);
	
	void	cap(string b, User &user);
	void	who(string b, User &user);
	void	join(string b, User &user);
	void	kick(string b, User &user);
	void	invite(string b, User &user);
	void	topic(string b, User &user);
	void	mode(string b, User &user);
//////////////////////////////////////////////////////
///////////////////////HELP///////////////////////////
	bool	startServ();
	void	add_client();

};

#endif
