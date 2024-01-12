#include "../includes/channel.hpp"

Channel::Channel(std::string n)
{
	name = n;
	max = 5;
	key = "";
	i = false;
	t = false;
	l = false;
}

Channel::Channel(Channel const &other)
{
	this->name = other.name;
	this->key = other.key;
	this->topic = other.topic;
	this->i = other.i;
	this->t = other.t;
	this->l = other.l;
	this->max = other.max;

	for (std::map<int, User*>::const_iterator i = other.members.begin(); i != other.members.end(); i++)
		this->members.insert(make_pair(i->first, i->second));
		// this->members.insert(make_pair(i->first, i->second));
	for (std::map<std::string, User>::const_iterator i = other.bans.begin(); i != other.bans.end(); i++)
		this->bans.insert(make_pair(i->first, i->second));
		// this->bans.insert(make_pair<std::string, User>(i->first, i->second));
	for (std::map<std::string, bool>::const_iterator i = other.oper.begin(); i != other.oper.end(); i++)
		this->oper.insert(make_pair(i->first, i->second));
		// this->oper.insert(make_pair<std::string, bool>(i->first, i->second));

}


std::string Channel::getChannelName(){return (name);}
void	Channel::setChannelName(std::string n){name = n;}

std::map<int, User*> &Channel::getMembers(){return (members);}
void	Channel::setMembers(int fd, User &user){members.insert(pair(fd, &user));}

void	Channel::setChannelKey(std::string k){key = k;}
std::string	Channel::getChannelKey(){return (key);}

void	Channel::setChannelTopic(std::string t){topic = t;}
std::string	Channel::getChannelTopic(){return (topic);}

map<int, User*>::iterator Channel::findUserFromChannel(std::string name)
{
	map<int, User*>::iterator i = members.begin();
	for (; i != members.end(); i++)
	{
		if (i->second->getNickName() == name)
			return i;
	}
	return members.end();
}