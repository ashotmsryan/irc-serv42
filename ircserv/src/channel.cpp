#include "../includes/channel.hpp"

Channel::Channel(std::string n)
{
	name = n;
	max = 5;
	i = false;
	t = false;
	l = false;
}

std::string Channel::getChannelName(){return (name);}
void	Channel::setChannelName(std::string n){name = n;}

std::map<int, User> &Channel::getMembers(){return (members);}
void	Channel::setMembers(int fd, User &user){members.insert(pair<int, User> (fd, user));}

void	Channel::setChannelKey(std::string k){key = k;}
std::string	Channel::getChannelKey(){return (key);}

void	Channel::setChannelTopic(std::string t){topic = t;}
std::string	Channel::getChannelTopic(){return (topic);}

map<int, User>::iterator Channel::findUserFromChannel(std::string name)
{
	map<int, User>::iterator i = members.begin();
	for (; i != members.end(); i++)
	{
		if (i->second.getNickName() == name)
			return i;
	}
	return members.end();
}