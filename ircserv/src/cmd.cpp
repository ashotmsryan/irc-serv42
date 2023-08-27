#include "../includes/serv.hpp"

void	serv::user(string b, User &user)
{
	if (!user.functionality)
	{
		size_t k = b.find_first_of(":");
		std::string rname;
		std::string s;
		std::vector<std::string> arr;
		if (k != string::npos)
		{
			rname = b.substr(k, b.size());
			s = b.substr(0, k);
			arr = split(s);
		}
		else
		{
			s = b;
			arr = split(s);
			if (!arr.empty())
				rname = arr.back();
		}
		if (arr.empty() || arr.size() < 3 || (arr.size() <= 3 && k == string::npos))
		{
			msg_err.ERR_NEEDMOREPARAMS(user.getUserFD(), "USER");
			return;
		}
		user.setUserName(arr[0]);
		user.setHostName(arr[1]);
		user.setServName(arr[2]);
		user.setRealName(rname);
		if (!user.functionality && user.getNickName() != "" && user.getPassFlag())
		{
			msg_err.RPL_REGISTER(user.getUserFD(), user.getNickName(), user.getUserName());
			user.functionality = true;
		}
	}
	else
		msg_err.ERR_ALREADYREGISTRED(user.getUserFD(), "");

}

void	serv::pass(string b, User &user)
{
	std::vector<string> arr = split(b);

	size_t i = b.find(':');
	
	if (arr.size() < 1)
	{
		msg_err.ERR_NEEDMOREPARAMS(user.getUserFD(), "PASS");
		return ;
	}
	if (user.getPassFlag())
	{
		msg_err.ERR_ALREADYREGISTRED(user.getUserFD(), user.getNickName());
		return ;
	}
	else if (arr[0] == password)
	{
		user.changePassFlag();
		send(user.getUserFD(), "PASS :You loged in\n", 19, 0);
		if (!user.functionality && !user.getNickName().empty() && !user.getHostName().empty())
		{
			msg_err.RPL_REGISTER(user.getUserFD(), user.getNickName(), user.getUserName());
			user.functionality = true;
		}
	}
	else if(i != std::string::npos && password.compare(&(arr[0].c_str())[i + 1]))
	{
		user.changePassFlag();
		send(user.getUserFD(), "PASS :You loged in\n", 19, 0);
		if (!user.functionality && !user.getNickName().empty() && !user.getHostName().empty())
		{
			msg_err.RPL_REGISTER(user.getUserFD(), user.getNickName(), user.getUserName());
			user.functionality = true;
		}
	}
}


void	serv::nick(string b, User &user)
{
	std::vector<string> arr = split(b);

	if (arr.empty())
	{
		msg_err.ERR_NEEDMOREPARAMS(user.getUserFD(), "NICK");
		return ;
	}
	if (arr[0].find_first_of(",@!:") != string::npos || arr[0].size() > MaxLenght)
	{
		msg_err.ERR_ERRONEUSNICKNAME(user.getUserFD(), user.getNickName());
		return;
	}
	for (size_t i = 0; i != arr[0].size(); i++)
	{
		if (!isprint(arr[0][i]))
		{
			msg_err.ERR_ERRONEUSNICKNAME(user.getUserFD(), user.getNickName());
			return;	
		}
	}
	if (arr.empty() || (!arr.empty() && arr[0].empty()))
	{
		msg_err.ERR_ERRONEUSNICKNAME(user.getUserFD(), user.getNickName());
		return;
	}
	if (!user.getNickName().empty())
	{
		if (findUserByNick(arr[0]) && findUserByNick(arr[0]) != user.getUserFD())
		{
			msg_err.ERR_NICKNAMEINUSE(user.getUserFD(), user.getNickName());
			return;
		}
		user.setNickName(arr[0]);
	}
	else
	{
		if (findUserByNick(arr[0]))
		{
			msg_err.ERR_NICKNAMEINUSE(user.getUserFD(), user.getNickName());
			return;
		}
		user.setNickName(arr[0]);
	}
	if(!user.functionality && !user.getHostName().empty() && user.getPassFlag())
	{
		msg_err.RPL_REGISTER(user.getUserFD(), user.getNickName(), user.getUserName());
		user.functionality = true;
	}
}

void	serv::ping(string b, User &use)
{
	if (use.getPassFlag())
	{
		std::vector<string> arr = split(b);

		if (arr.empty() || (!arr.empty() && arr[0].empty()))
			msg_err.ERR_NOORIGIN(use.getUserFD(), "PING");
		else
			pong(arr[0], use);
	}
}

void	serv::pong(string b, User &user)
{
	std::vector<string> arr = split(b);

	if (arr.empty() || (!arr.empty() && arr[0].empty()))
	{
		msg_err.ERR_NOORIGIN(user.getUserFD(), "PONG");
		return;
	}
	msg_err.RPL_PONG(user.getUserFD(), user.getNickName());
}

void	serv::cap(string b, User &user)
{
	b = "";
	cout<< "cap called by fd "<< user.getUserFD() << endl;
}

void	serv::quit(string b, User &user) // delete from channels
{
	if (b.empty())
		msg_err.RPL_QUIT(user.getUserFD(), user.getNickName(), "");
	else
		msg_err.RPL_QUIT(user.getUserFD(), user.getNickName(), b);
	// getpeername(user.getUserFD(), (struct sockaddr*)(&def), (socklen_t*)&addrlen);
	FD_CLR(user.getUserFD(), &def);
	removeFromChannels(user.getUserFD());
	cout << user.getUserFD() << endl;
	close(user.getUserFD());
	users.erase(user.getUserFD());
}

void	serv::privmsg(string b, User &user)
{
	string nick;
	string msg;
	size_t sp = b.find(":");

	if (sp == string::npos)
	{
		msg_err.ERR_NOTEXTTOSEND(user.getUserFD(), "PRIVMSG");
		return ;
	}
	nick = b.substr(0, sp);
	msg = b.substr(sp, b.size() - sp);
	msg = msg.substr(msg.find(":"), msg.size());
	std::vector<std::string> part = split(nick);
	if (part.empty())
	{
		msg_err.ERR_NORECIPIENT(user.getUserFD(), "PRIVMSG");
		return ;
	}
	else if (part.size() != 1)
	{
		msg_err.ERR_TOOMANYTARGETS(user.getUserFD(), part[part.size() - 1]);
		return ;
	}
	else if (msg.empty())
	{
		msg_err.ERR_NOTEXTTOSEND(user.getUserFD(), "PRIVMSG");
		return ;
	}
	else if (user.getChannels().find(part[0]) == user.getChannels().end())
	{
		if (!findUserByNick(part[0]))
		{
			msg_err.ERR_NOSUCHNICK(user.getUserFD(), part[0]);
			return ;
		}
		else
		{
			msg_err.RPL_PRIVMSG(findUserByNick(part[0]), user.getNickName(), user.getUserName(), part[0], msg);
			return;
		}
		msg_err.ERR_CANNOTSENDTOCHAN(user.getUserFD(), part[0]);
		return ;
	}
	std::map<std::string, Channel&>::iterator i = user.getChannels().find(part[0]);
	std::string prefix = ":" + user.getNickName() + "!" + user.getUserName() +  "@localhost PRIVMSG ";
	for (std::map<int, User&>::iterator it = i->second.getMembers().begin(); it != i->second.getMembers().end(); it++)
	{
		std::string m = prefix + i->second.getChannelName() + " " + msg + "\n";
		if (it->second.getUserFD() != user.getUserFD())
			send(it->second.getUserFD(), m.c_str(), m.size() ,0);
	}
}

void	serv::join(string b, User &user)
{
	std::vector<string> arr = split(b);

	if (arr.empty() || (!arr.empty() && arr[0].empty()))
	{
		msg_err.ERR_NEEDMOREPARAMS(user.getUserFD(), "JOIN");
		return ;
	}
	if (arr[0][0] != '#')
		arr[0] = '#' + arr[0];
	map<std::string, Channel>::iterator i = all_channels.find(arr[0]);
	if (i != all_channels.end())
	{
		if (!i->second.i)
		{
			cout << "2" << endl;
			joinChannel(user, i->second, arr, false);
		}
		else
		{
			msg_err.ERR_INVITEONLYCHAN(user.getUserFD(), arr[0]);
			return ;
		}
	}
	else
	{
		cout << "1" << endl;
		Channel chan(arr[0]);
		chan.oper.insert(std::pair<std::string, bool> (user.getNickName(), true));
		joinChannel(user, chan, arr, true);
	}
	
}

void	serv::kick(std::string b, User &user)
{
	std::vector<std::string> arr = split(b);

	if (arr.size() < 2)
	{
		msg_err.ERR_NEEDMOREPARAMS(user.getUserFD(), "KICK");
		return ;
	}
	std::map<std::string, Channel&>::iterator i = user.getChannels().find(arr[0]);
	if (i != user.getChannels().end())
	{
		if (i->second.oper.find(user.getNickName()) != i->second.oper.end())
		{
			std::map<int, User&>::iterator it = i->second.getMembers().find(findUserByNick(arr[1]));
			if (it == i->second.getMembers().end())
			{
				msg_err.ERR_NOSUCHNICK(user.getUserFD(), arr[1]);
				return ;
			}
			msg_err.RPL_KICK(it->first, user.getNickName(), user.getUserName(), it->second.getNickName(), i->first);
			std::string prefix = ":" + user.getNickName() + "!" + user.getUserName() + "@localhost KICK ";
			std::string msg = i->first + " " + it->second.getNickName() + "\n";
			sendAll(i->second.getMembers(), prefix, msg);
			i->second.getMembers().erase(it);
			if (i->second.getMembers().empty())
				all_channels.erase(i->second.getChannelName());
		}
		else
		{
			msg_err.ERR_CHANOPRIVSNEEDED(user.getUserFD(), arr[0]);
			return ;
		}
	}
	else
	{
		msg_err.ERR_NOSUCHCHANNEL(user.getUserFD(), arr[0]);
		return ;
	}
}

void	serv::invite(std::string b, User &user)
{
	std::vector<std::string> arr = split(b);
	if (arr.size() < 2)
	{
		msg_err.ERR_NEEDMOREPARAMS(user.getUserFD(), "INVITE");
		return ;
	}
	std::map<std::string, Channel&>::iterator ch = user.getChannels().find(arr[1]);
	if (ch == user.getChannels().end())
	{
		msg_err.ERR_NOTONCHANNEL(user.getUserFD(), arr[0]);
		return ;
	}
	if (!findUserByNick(arr[0]))
	{
		msg_err.ERR_NOSUCHNICK(user.getUserFD(), arr[0]);
		return ;
	}
	std::map<int, User>::iterator it = users.find(findUserByNick(arr[0]));
	if (it == users.end())
	{
		msg_err.ERR_USERONCHANNEL(user.getUserFD(), arr[1], arr[0]);
		return ;
	}
	cout << it->first << endl;
	if (ch->second.i && ch->second.oper.find(user.getNickName()) == ch->second.oper.end())
	{
		msg_err.ERR_CHANOPRIVSNEEDED(user.getUserFD(), arr[1]);
		return ;
	}
	cout << it->second.getUserFD() << endl;
	ch->second.setMembers(findUserByNick(arr[0]), users.find(findUserByNick(arr[0]))->second);
	it->second.setChannels(ch->second.getChannelName(), ch->second);
	msg_err.RPL_INVITING(user.getUserFD(), ch->second.getChannelName(), user.getNickName(), user.getUserName(), arr[0]);
}

void	serv::topic(std::string b, User &user)
{
	std::vector<std::string> arr = split(b);
	if (arr.size() == 0)
	{
		msg_err.ERR_NEEDMOREPARAMS(user.getUserFD(), "JOIN");
		return ;
	}
	std::map<std::string, Channel&>::iterator it = user.getChannels().find(arr[0]);
	if (it == user.getChannels().end())
	{
		msg_err.ERR_NOTONCHANNEL(user.getUserFD(), arr[0]);
		return ;
	}
	if (arr.size() == 1)
	{
		if (it->second.getChannelTopic().empty())
		{
			msg_err.RPL_TOPIC(user.getUserFD(),it->second.getChannelName(), true, "");
			return ;
		}
		msg_err.RPL_TOPIC(user.getUserFD(),it->second.getChannelName(), false, it->second.getChannelTopic());
	}
	else
	{
		size_t d = b.find(':');
		std::string t;
		if (d != std::string::npos)
			t = b.substr(b.find(':') + 1);
		else
			t = arr[1];
		if (it->second.t && it->second.oper.find(user.getNickName()) != it->second.oper.end())
			it->second.setChannelTopic(t);
		else if (!it->second.t)
			it->second.setChannelTopic(t);
		else
		{
			msg_err.ERR_CHANOPRIVSNEEDED(user.getUserFD(), arr[1]);
			return ;
		}
	}
}

void	serv::mode(string b, User &user)
{
	std::vector<std::string> arr = split(b);
	if (arr.size() < 1)
	{
		msg_err.ERR_NEEDMOREPARAMS(user.getUserFD(), "MODE");
		return ;
	}
	std::map<std::string, Channel&>::iterator it = user.getChannels().find(arr[0]);
	if (it == user.getChannels().end())
	{
		msg_err.ERR_NOTONCHANNEL(user.getUserFD(), arr[0]);
		return ;
	}
	if (arr.size() == 1)
		sendModeReply(user, it->second);
	else
	{
		if ((it->second.oper.find(user.getNickName())) == it->second.oper.end() && arr[1][0] != 'b')
		{
			msg_err.ERR_CHANOPRIVSNEEDED(user.getUserFD(), arr[0]);
			return ;
		}
		if ((arr[1].size() == 2 && (arr[1][0] != '-' && arr[1][0] != '+') && arr[1][1] != 'i'
			&& arr[1][1] != 't' && arr[1][1] != 'k' && arr[1][1] != 'o' && arr[1][1] != 'l')
			|| (arr[1].size() == 1 && arr[1][0] != 'b' && arr[1][0] != 'i' && arr[1][0] != 't'
			&& arr[1][0] != 'k' && arr[1][0] != 'o' && arr[1][0] != 'l'))
		{
			cout << "llllllll" << endl;
			msg_err.ERR_UNKNOWNMODE(user.getUserFD(), arr[1]);
			return ;
		}
		int i = -1;
		array<string, 5> mode = {"i", "t", "k", "o", "l"};
		if (arr[1].size() == 1)
			while (++i != 5 && mode[i][0] != arr[1][0]);
		else if (arr[1].size() > 1)
			while (++i != 5 && mode[i][0] != arr[1][1]);
		switch (i)
		{
		case 0: // i (invite only channel)
			if (arr[1][0] == '+' || arr[1][0] == 'i')
			{
					it->second.i = true;
				cout << "hhhhhhhhhhhhhhhh" << endl;
			}
			else if (arr[1][0] == '-')
				it->second.i = false;
			break;
		case 1: // t (show topic or not)
			if (arr[1][0] == '+' || arr[1][0] == 't')
				it->second.t = true;
			else if (arr[1][0] == '-')
				it->second.t = false;
			break;
		case 2: // k (set/delete key)
			if (arr[1][0] == '+' || arr[1][0] == 'k')
			{
				if (arr.size() >= 3)
						it->second.setChannelKey(arr[2]);
				else
				{
					msg_err.ERR_NEEDMOREPARAMS(user.getUserFD(), "MODE");
					return ;
				}
			}
			else if (arr[1][0] == '-')
				it->second.setChannelKey("");
			break;
		case 3: // o (give operator)
			if (arr.size() >= 3)
			{
				if (arr[1][0] == '+' || arr[1][0] == 'o')
				{
					if (it->second.findUserFromChannel(arr[2]) != it->second.getMembers().end())
					{
						it->second.oper[arr[2]] = true;
					}
					else
					{
						msg_err.ERR_NOTONCHANNEL(user.getUserFD(), arr[0]);
						return ;
					}
				}
				else if (arr[1][0] == '-')
				{
					if (it->second.findUserFromChannel(arr[2]) != it->second.getMembers().end())
						it->second.oper.erase(arr[2]);
					else
					{
						msg_err.ERR_NOTONCHANNEL(user.getUserFD(), arr[0]);
						return ;
					}
				}
			}
			else
			{
				msg_err.ERR_NEEDMOREPARAMS(user.getUserFD(), "MODE");
				return ;
			}
			break;
		case 4: // l (set member limit on channel)
			if (arr[1][0] == '+' || arr[1][0] == 'l')
			{
				if (arr.size() >= 3)
				{
					if (checkNumber(arr[2]))
					{
						it->second.l = true;
						int i = std::atoi(arr[2].c_str());
						if (i >= 0 && i < 10)
							it->second.max = std::atoi(arr[2].c_str());
						else
						{
							send(user.getUserFD(), ":MODE@localhost Invalid limit\n", 31, 0);
							return ;
						}
					}
				}
				else
				{
					msg_err.ERR_NEEDMOREPARAMS(user.getUserFD(), "MODE");
					return ;
				}
			}
			else if (arr[1][0] == '-')
			{
				it->second.l = false;
				it->second.max = 0;
			}
			break;
		}
	}
}


void	serv::notice(std::string b, User &user)
{
	string nick;
	string msg;
	size_t sp = b.find(":");

	if (sp == string::npos)
		return ;
	nick = b.substr(0, sp);
	msg = b.substr(sp, b.size());
	msg = msg.substr(msg.find(":"), msg.size());
	std::vector<std::string> part = split(nick);
	if (part.empty() || part.size() != 1 || msg.empty())
		return ;
	else if (user.getChannels().find(part[0]) == user.getChannels().end())
	{
		if (!findUserByNick(part[0]))
			return ;
		else
		{
			msg_err.RPL_PRIVMSG(findUserByNick(part[0]), user.getNickName(), user.getUserName(), part[0], msg);
			return;
		}
		return ;
	}
	std::map<std::string, Channel&>::iterator i = user.getChannels().find(part[0]);
	std::string prefix = ":" + user.getNickName() + "!" + user.getUserName() +  "@localhost NOTICE ";
	for (std::map<int, User&>::iterator it = i->second.getMembers().begin(); it != i->second.getMembers().end(); it++)
	{
		std::string m = prefix + it->second.getNickName() + " " + msg;
		if (it->second.getUserFD() != user.getUserFD())
			send(it->second.getUserFD(), m.c_str(), m.size() ,0);
	}
}

void	serv::who(string b, User &user)
{
	std::vector<std::string> arr = split(b);
	if (!arr.empty())
	{
		std::map<std::string, Channel>::iterator c = all_channels.find(arr[0]);
		if (c != all_channels.end())
		{
			std::map<int, User&>::iterator i = c->second.getMembers().begin();
			for (; i != c->second.getMembers().end(); i++)
				msg_err.RPL_WHOREPLY(user.getUserFD(), user.getNickName(), c->second.getChannelName(), i->second.getNickName(), user.getUserName());
			msg_err.RPL_ENDOFWHO(user.getUserFD(), user.getNickName(), c->second.getChannelName());
		}
		else
		{
			msg_err.ERR_NOSUCHCHANNEL(user.getUserFD(), arr[0]);
			return ;
		}
	}
}

void	serv::part(string b, User &user)
{
	std::vector<std::string> arr = split(b);

	if (arr.size() < 1)
	{
		msg_err.ERR_NEEDMOREPARAMS(user.getUserFD(), "PART");
		return ;
	}
	std::map<std::string, Channel>::iterator i = all_channels.find(arr[0]);
	if (i == all_channels.end())
	{
		msg_err.ERR_NOSUCHCHANNEL(user.getUserFD(), arr[0]);
		return ;
	}
	std::map<int, User&>::iterator j = (i->second.getMembers()).find(user.getUserFD());
	if (j == (i->second.getMembers()).end())
	{
		msg_err.ERR_NOTONCHANNEL(user.getUserFD(), arr[0]);
		return ;
	}
	i->second.getMembers().erase(j->first);
	j = i->second.getMembers().begin();
	for (; j != i->second.getMembers().end(); j++)
		msg_err.RPL_PART(j->second.getUserFD(), user.getNickName(), user.getUserName(), arr[0]);
	msg_err.RPL_PART(user.getUserFD(), user.getNickName(), user.getUserName(), arr[0]);
	std::map<std::string, bool>::iterator o = i->second.oper.find(user.getNickName());
	if (o != i->second.oper.end())
		i->second.oper.erase(user.getNickName());
}
