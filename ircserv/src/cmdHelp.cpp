#include "../includes/serv.hpp"

void	serv::sendAll(std::map<int, User> use, std::string cmd, std::string msg)
{
	for (std::map<int, User>::iterator i = use.begin(); i != use.end(); i++)
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

map<std::string, Channel>::iterator serv::findChannelsFromUsers(std::string name)
{
	map<int, User>::iterator i = users.begin();
	map<std::string, Channel>::iterator it;
	for (; i != users.end(); i++)
	{
		it = i->second.getChannels().find(name);
		if (it->first == name)
			return it;
	}
	i = users.begin();
	it = i->second.getChannels().end();
	return it;
}

void	serv::sendReplyToJoin(Channel &chan, User &user)
{
	std::map<int, User>::iterator i = chan.getMembers().begin();
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
	sendAll(chan.getMembers(), (":" + user.getNickName() + "@localhost JOIN "),  (chan.getChannelName() + "\n"));
	// if (chan.getChannelTopic().empty())
	// 	msg_err.RPL_TOPIC(user.getUserFD(), chan.getChannelName(), false, "");
	// else
	// 	msg_err.RPL_TOPIC(user.getUserFD(), chan.getChannelName(), true, chan.getChannelTopic());
	msg_err.RPL_NAMREPLY(user.getUserFD(), user.getNickName(), chan.getChannelName(), oper, members);
	msg_err.RPL_ENDOFNAMES(user.getUserFD(), user.getNickName(), chan.getChannelName());

}


bool	serv::joinWithTwoArgs(User &user, Channel &chan, std::vector<string> arr, bool flag)
{
	if (flag)
	{
		if (chan.oper.find(user.getNickName()) == chan.oper.end())
		{
			chan.setChannelKey(arr[1]);
			if (arr[0][0] != '#')
				arr[0] = '#' + arr[0];
			chan.setChannelName(arr[0]);
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
	chan.setMembers(user.getUserFD(), user);
	user.setChannels(chan.getChannelName(), chan);
}
