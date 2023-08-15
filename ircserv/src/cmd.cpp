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
			rname = arr.back();
		}
		if (arr.size() < 3 || (arr.size() <= 3 && k == string::npos))
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
			msg_err.RPL_REGISTER(user.getUserFD(), user.getNickName());
			user.functionality = true;
		}
	}
	else
		msg_err.ERR_ALREADYREGISTRED(user.getUserFD(), "");

}

void	serv::pass(string b, User &user)
{
	std::vector<string> arr = split(b);

	if (arr.size() < 1)
	{
		msg_err.ERR_NEEDMOREPARAMS(user.getUserFD(), "PASS");
		return ;
	}
	else if (arr[0] == password)
	{
		if (!user.getPassFlag())
		{
			user.changePassFlag();
			send(user.getUserFD(), "PASS :You loged in\n", 19, 0);
		}
		else
		{
			msg_err.ERR_ALREADYREGISTRED(user.getUserFD(), user.getNickName());
			return ;
		}
		if (!user.functionality && !user.getNickName().empty() && !user.getHostName().empty())
		{
			msg_err.RPL_REGISTER(user.getUserFD(), user.getNickName());
			user.functionality = true;
		}
	}
}


void	serv::nick(string b, User &user)
{
	std::vector<string> arr = split(b);

	if (arr.size() > 2 || arr[0].find_first_of(",@!:") != string::npos || arr[0].size() > MaxLenght)
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
		msg_err.RPL_REGISTER(user.getUserFD(), user.getNickName());
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
	getpeername(user.getUserFD(), (struct sockaddr*)(&def), (socklen_t*)&addrlen);
	FD_CLR(user.getUserFD(), &def);
	close(user.getUserFD());
	users.erase(user.getUserFD());
	cout << "##########################################################################" <<std::endl;
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
	msg = b.substr(sp, b.size());
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
			msg_err.RPL_PRIVMSG(findUserByNick(part[0]), user.getNickName(), part[0], msg);
			return;
		}
		msg_err.ERR_CANNOTSENDTOCHAN(user.getUserFD(), part[0]);
		return ;
	}
	std::map<std::string, Channel>::iterator i = user.getChannels().find(part[0]);
	for (std::map<int, User>::iterator it = i->second.getMembers().begin(); it != i->second.getMembers().end(); it++)
	{
		if (it->second.getUserFD() != user.getUserFD())
			send(it->second.getUserFD(), msg.c_str(), msg.size() ,0);
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
	map<std::string, Channel>::iterator i = findChannelsFromUsers(arr[0]);
	if (i != (users.begin())->second.getChannels().end())
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
	std::map<std::string, Channel>::iterator i = user.getChannels().find(arr[0]);
	if (i != user.getChannels().end())
	{
		if (i->second.oper.find(user.getNickName()) != i->second.oper.end())
		{
			std::map<int, User>::iterator it = i->second.getMembers().find(findUserByNick(arr[1]));
			if (it == i->second.getMembers().end())
			{
				msg_err.ERR_NOSUCHNICK(user.getUserFD(), arr[1]);
				return ;
			}
			sendAll(i->second.getMembers(), "KICK", "kick " + it->second.getNickName() + " from " + i->second.getChannelName());
			i->second.getMembers().erase(it);
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
	std::map<std::string, Channel>::iterator ch = user.getChannels().find(arr[1]);
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
	std::map<int, User>::iterator it = ch->second.getMembers().find(findUserByNick(arr[0]));
	if (it != ch->second.getMembers().end())
	{
		msg_err.ERR_USERONCHANNEL(user.getUserFD(), arr[1], arr[0]);
		return ;
	}
	if (ch->second.i && ch->second.oper.find(user.getNickName()) == ch->second.oper.end())
	{
		msg_err.ERR_CHANOPRIVSNEEDED(user.getUserFD(), arr[1]);
		return ;
	}
	cout << it->second.getUserFD() << endl;
	ch->second.setMembers(findUserByNick(arr[0]), users.find(findUserByNick(arr[0]))->second);
	users.find(findUserByNick(arr[0]))->second.setChannels(ch->second.getChannelName(), ch->second);
	// it->second.setChannels(ch->second.getChannelName(), ch->second);
	// sendAll(ch->second.getMembers(), ":INVITE@localhost", "the " + arr[0] + " entered to the channel"); 
	msg_err.RPL_INVITING(user.getUserFD(), ch->second.getChannelName(), user.getNickName());
}

void	serv::topic(std::string b, User &user)
{
	std::vector<std::string> arr = split(b);
	if (arr.size() == 0)
	{
		msg_err.ERR_NEEDMOREPARAMS(user.getUserFD(), "JOIN");
		return ;
	}
	std::map<std::string, Channel>::iterator it = user.getChannels().find(arr[0]);
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
		if (!it->second.t && it->second.oper.find(user.getNickName()) != it->second.oper.end())
		{
			std::string t = b.substr(b.find(':') + 1);
			it->second.setChannelTopic(t);
		}
		else if (it->second.t)
		{
			std::string t = b.substr(b.find(':') + 1);
			it->second.setChannelTopic(t);
		}
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
	if (arr.size() < 2)
	{
		msg_err.ERR_NEEDMOREPARAMS(user.getUserFD(), "MODE");
		return ;
	}
	std::map<std::string, Channel>::iterator it = user.getChannels().find(arr[0]);
	if (it == user.getChannels().end())
	{
		msg_err.ERR_NOTONCHANNEL(user.getUserFD(), arr[0]);
		return ;
	}


	map<std::string, bool>::iterator m = it->second.oper.begin();
	for (; m != it->second.oper.end(); m++)
		cout << "opers = " << m->first <<endl;
	map<int, User>::iterator a = it->second.getMembers().begin();
	for (; a != it->second.getMembers().end(); a++)
		cout << "members = " << a->first <<endl;
	
	
	if ((it->second.oper.find(user.getNickName())) == it->second.oper.end())
	{
		msg_err.ERR_CHANOPRIVSNEEDED(user.getUserFD(), arr[0]);
		return ;
	}
	if (arr[1].size() != 2 || (arr[1][0] != '+' && arr[1][0] != '-') || (arr[1][1] != 'i'
		&& arr[1][1] != 't' && arr[1][1] != 'k' && arr[1][1] != 'o' && arr[1][1] != 'l'))
	{
		msg_err.ERR_UNKNOWNMODE(user.getUserFD(), arr[1]);
		return ;
	}
	int i = -1;
	array<string, 5> mode = {"i", "t", "k", "o", "l"};
	while (++i != 5 && mode[i][0] != arr[1][1]);
	switch (i)
	{
	case 0: // i (invite only channel)
		if (arr[1][0] == '+')
			it->second.i = true;
		else if (arr[1][0] == '-')
			it->second.i = false;
		break;
	case 1: // t (show topic or not)
		if (arr[1][0] == '+')
			it->second.t = true;
		else if (arr[1][0] == '-')
			it->second.t = false;
		break;
	case 2: // k (set/delet key)
		if (arr[1][0] == '+')
		{
			cout << arr.size() << endl;
			if (arr.size() >= 3)
			{
				if (it->second.findUserFromChannel(arr[2]) != it->second.getMembers().end())
					it->second.setChannelKey(arr[2]);
				else
				{
					msg_err.ERR_NOTONCHANNEL(user.getUserFD(), arr[0]);
					return ;
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
			if (it->second.findUserFromChannel(arr[2]) != it->second.getMembers().end())
				it->second.setChannelKey("");
			else
			{
				msg_err.ERR_NOTONCHANNEL(user.getUserFD(), arr[0]);
				return ;
			}
		}
		break;
	case 3: // o (give operator)
		if (arr.size() >= 3)
		{
			if (arr[1][0] == '+')
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
		if (arr[1][0] == '+')
		{
			if (arr.size() >= 3)
			{
				if (it->second.findUserFromChannel(arr[2]) != it->second.getMembers().end())
				{
					it->second.l = true;
					int i = std::atoi(arr[2].c_str());
					if (i >= 0 && i < 10)
						it->second.max = std::atoi(arr[2].c_str());
					else
					{
						send(user.getUserFD(), ":MODE@localhost Invalid limit\n", 20, 0);
						return ;
					}
				}
				else
				{
					msg_err.ERR_NOTONCHANNEL(user.getUserFD(), arr[0]);
					return ;
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
			if (it->second.findUserFromChannel(arr[2]) != it->second.getMembers().end())
			{
				it->second.l = false;
				it->second.max = 0;
			}
			else
			{
				msg_err.ERR_NOTONCHANNEL(user.getUserFD(), arr[0]);
				return ;
			}
		}
		break;
	}
}

