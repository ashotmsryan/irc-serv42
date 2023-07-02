#pragma once

#include "serv.hpp"

class Channel
{
	std::map<int, User> members;
public:
	std::map<int, User> &getChannels();
	void	setChannels(int fd, User &user);
};