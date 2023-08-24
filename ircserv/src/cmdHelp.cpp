#include "../includes/serv.hpp"

void	serv::sendAll(std::map<int, User&> use, std::string cmd, std::string msg)
{
	for (std::map<int, User&>::iterator i = use.begin(); i != use.end(); i++)
	{
		send(i->second.getUserFD(), (cmd + msg).c_str(), cmd.size() + msg.size(), 0);
	}
}

bool	serv::checkChannelNameKey(std::vector<std::string> arr)
{
	if (arr[0][0] != '#')
		return false;
	return true;
}

map<std::string, Channel&>::iterator serv::findChannelsFromUsers(std::string name)
{
	map<int, User>::iterator i = users.begin();
	map<std::string, Channel&>::iterator it;
	for (; i != users.end(); i++)
	{
		it = i->second.getChannels().find(name);
		if (it->first == name)
			return it;
	}
	it = (users.begin())->second.getChannels().end();
	return it;
}

bool	serv::joinWithTwoArgs(User &user, Channel &chan, std::vector<string> arr, bool flag)
{
	if (flag)
	{
		if (chan.oper.find(user.getNickName()) != chan.oper.end())
		{
			chan.setChannelKey(arr[1]);
			if (arr[0][0] != '#')
				arr[0] = '#' + arr[0];
			chan.setChannelName(arr[0]);
			sendReplyToJoin(chan, user);
		}
		else
		{
			msg_err.ERR_CHANOPRIVSNEEDED(user.getUserFD(), arr[0]);
			return true;
		}
	}
	else if (chan.getChannelKey() == arr[1])
	{
		if (chan.bans.find(user.getNickName()) != chan.bans.end())
		{
			msg_err.ERR_BANNEDFROMCHAN(user.getUserFD(), arr[0]);
			return true;
		}
		else
		{
			if ((chan.l && chan.max > chan.getMembers().size()) || !chan.l)
			{
				if (arr[0][0] == '#')
					sendReplyToJoin(chan, user);
				else
				{
					msg_err.ERR_NOSUCHCHANNEL(user.getUserFD(), arr[0]);
					return true;
				}
			}
			else
			{
				msg_err.ERR_CHANNELISFULL(user.getUserFD(), arr[0]);
				return true;
			}
		}
	}
	else
	{
		msg_err.ERR_CHANNELISFULL(user.getUserFD(), arr[0]);
		return true;
	}
	return false;
}

bool	serv::joinWithOneArgs(User &user, Channel &chan, std::vector<string> arr, bool flag)
{
	if (flag)
	{
		if (arr[0][0] != '#')
			arr[0] = '#' + arr[0];
		fromJoinCheck = true;
		chan.setChannelName(arr[0]);
	}
	if (chan.bans.find(user.getNickName()) != chan.bans.end())
	{
		msg_err.ERR_BANNEDFROMCHAN(user.getUserFD(), arr[0]);
		return true;
	}
	else
	{
		if ((chan.l && chan.max > chan.getMembers().size()) || !chan.l)
		{
			if (chan.getChannelKey().empty())
				sendReplyToJoin(chan, user);
			else
			{
				msg_err.ERR_NEEDMOREPARAMS(user.getUserFD(), arr[0]);
				return true;
			}
		}
		else
		{
			msg_err.ERR_CHANNELISFULL(user.getUserFD(), arr[0]);
			return true;
		}
	}
	return (false);
}

void	serv::joinChannel(User &user, Channel &chan, std::vector<string> arr, bool flag)
{
	if (arr.size() >= 2)
	{
		if (joinWithTwoArgs(user, chan, arr, flag))
			return ;
	}
	else if (arr.size() == 1 && chan.getChannelKey().empty())
	{
		if (joinWithOneArgs(user, chan, arr, flag))
			return ;
	}
	else
		return ;
	all_channels.insert(std::make_pair<std::string, Channel>(chan.getChannelName(), chan));
	all_channels.find(chan.getChannelName())->second.setMembers(user.getUserFD(), user);
	user.setChannels(all_channels.find(chan.getChannelName())->first, all_channels.find(chan.getChannelName())->second);
}


bool serv::checkNumber(std::string n)
{
	int i = atoi(n.c_str());
	std::stringstream ss;
    ss << i;
    std::string str = ss.str();
	if (str == n)
		return (true);
	return false;
}

void	serv::removeFromChannels(int fd)
{
	map<std::string, Channel>::iterator i = all_channels.begin();
	map<int, User&>::iterator u;
	for(; i != all_channels.end(); i++)
	{
		u = i->second.getMembers().find(fd);
		if (u != i->second.getMembers().end())
		{
			if (i->second.oper.find(users.find(fd)->second.getNickName()) != i->second.oper.end())
				i->second.oper.erase(i->second.oper.find(users.find(fd)->second.getNickName()));
			i->second.getMembers().erase(u);
			if (i->second.getMembers().empty())
				all_channels.erase(i);
		}
		if (all_channels.empty())
			break ;
	}
}

int	serv::findUserByNick (string nick)
{
	map<int, User>::iterator e = users.end();
	for (map<int, User>::iterator i = users.begin(); i != e; i++)
	{
		if (i->second.getNickName() == nick)
			return (i->first);
	}
	return (0);
}

void	serv::sendReplyToJoin(Channel &chan, User &user)
{
	std::map<int, User&>::iterator i = chan.getMembers().begin();
	std::string members;
	std::string oper;
	for (; i != chan.getMembers().end(); i++)
	{
		if (chan.oper.find(i->second.getNickName()) != chan.oper.end())
		{
			oper.append("@");
			oper.append(i->second.getNickName());
			oper.append(" ");
		}
		else
		{
			members.append("+");
			members.append(i->second.getNickName());						
			members.append(" ");
		}
	}
	if (chan.getMembers().size() == 0)
	{
		oper.append("@");
		oper.append(user.getNickName());
		oper.append(" ");
	}
	else if (chan.getMembers().find(user.getUserFD()) == chan.getMembers().end())
	{
		members.append("+");
		members.append(user.getNickName());						
		members.append(" ");
	}
	send(user.getUserFD(), (":" + user.getNickName() + "!" + user.getUserName() + "@127.0.0.1 JOIN " + chan.getChannelName() + "\n").c_str(), (user.getNickName().size() + user.getUserName().size() + chan.getChannelName().size() + 20), 0);
	sendAll(chan.getMembers(), (":" + user.getNickName() + "@127.0.0.1 JOIN "),  (chan.getChannelName() + "\n"));
	// if (chan.getChannelTopic().empty())
	// 	msg_err.RPL_TOPIC(user.getUserFD(), chan.getChannelName(), false, "");
	// else
	// 	msg_err.RPL_TOPIC(user.getUserFD(), chan.getChannelName(), true, chan.getChannelTopic());
	msg_err.RPL_NAMREPLY(user.getUserFD(), user.getNickName(), chan.getChannelName(), oper, members);
	msg_err.RPL_ENDOFNAMES(user.getUserFD(), user.getNickName(), chan.getChannelName());

}
