#include "../includes/serv.hpp"

void	serv::sendAll(std::map<int, User> use, std::string cmd, std::string msg)
{
	for (std::map<int, User>::iterator i = use.begin(); i != use.end(); i++)
	{
		send(i->second.getUserFD(), (cmd + " :" + msg).c_str(), cmd.size() + msg.size() + 23, 0);
	}
}

bool	serv::checkChannelNameKey(std::vector<std::string> arr, bool flag)
{
	if (flag && ((arr[0][0] != '&') || (arr[0][0] == '&' && arr[1][0] != '#')))
		return false;
	else if (!flag && arr[0][0] != '#')
		return false;
	return true;
}

bool	serv::joinWithTwoArgs(User &user, Channel &chan, std::vector<string> arr, bool flag)
{
	if (chan.getChannelKey().empty() && flag)
	{
		chan.setChannelKey(arr[1]);
		chan.setChannelName(arr[0]);
	}
	else if (chan.getChannelKey() == arr[1])
	{
		if (chan.bans.find(user.getNickName()) != chan.bans.end())
		{
				// ERR_BANNEDFROMCHAN
				send(user.getUserFD(), "JOIN : Banned from channel\n", 28, 0);
				return true;
		}
		else
		{
			if (chan.max > chan.getMembers().size())
			{
				if (checkChannelNameKey(arr, true))
					send(user.getUserFD(), ("JOIN : the " + user.getNickName() + "joind the channel\n").c_str(), user.getNickName().size() + 31, 0);
				else
				{
					//  ERR_NOSUCHCHANNEL
					send(user.getUserFD(), "JOIN : No such a channel\n", 26, 0);
					return true;
				}
			}
			else
			{
				//  ERR_CHANNELISFULL
				send(user.getUserFD(), "JOIN : Channel is full\n", 24, 0);
				return true;
			}
		}
	}
	else
	{
		// ERR_BADCHANNELKEY
		send(user.getUserFD(), "JOIN : Bad channel key\n", 24, 0);
		return true;
	}
	return false;
}

bool	serv::joinWithOneArgs(User &user, Channel &chan, std::vector<string> arr, bool flag)
{
	if (chan.getChannelKey().empty() && flag)
	{
		chan.setChannelName(arr[0]);
	}
	if (chan.bans.find(user.getNickName()) != chan.bans.end())
	{
		// ERR_BANNEDFROMCHAN
		send(user.getUserFD(), "JOIN : Banned from channel\n", 28, 0);
		return true;
	}
	else
	{
		if (chan.max > chan.getMembers().size())
		{
			if (checkChannelNameKey(arr, false))
				send(user.getUserFD(), ("JOIN : the " + user.getNickName() + "joind the channel\n").c_str(), user.getNickName().size() + 31, 0);
			else
			{
				//  ERR_NOSUCHCHANNEL
				send(user.getUserFD(), "JOIN : No such a channel\n", 26, 0);
				return true;
			}
		}
		else
		{
			//  ERR_CHANNELISFULL
			send(user.getUserFD(), "JOIN : Channel is full\n", 24, 0);
			return true;
		}
	}
	return (false);
}

void	serv::joinChannel(User &user, Channel &chan, std::vector<string> arr, bool flag)
{
	if (arr.size() == 2)
	{
		if (joinWithTwoArgs(user, chan, arr, flag))
			return ;
	}
	else if (arr.size() == 1)
	{
		if (joinWithOneArgs(user, chan, arr, flag))
			return ;
	}
	else
	{
		send(user.getUserFD(), "JOIN :Too many parameters\n", 27, 0);
		return ;
	}
	std::map<int, User>::iterator i = users.find(user.getUserFD());
	chan.setMembers(user.getUserFD(), user);
	i->second.setChannels(chan.getChannelName(), chan);
}