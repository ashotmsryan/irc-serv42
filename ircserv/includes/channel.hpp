#pragma once

#include "serv.hpp"

class Channel
{
	std::map<int, User> members;
	std::string name;
	std::string key;
	std::string topic;
public:
	bool	i;
	int		max;
	std::map<std::string, User> bans;
	std::map<std::string, bool> oper;
	
	Channel(std::string n);
	
	std::string getChannelName();
	void setChannelName(std::string k);

	std::map<int, User> &getMembers();
	void	setMembers(int fd, User &user);

	void	setChannelKey(std::string k);
	std::string	getChannelKey();

	void	setChannelTopic(std::string t);
	std::string getChannelTopic();
};