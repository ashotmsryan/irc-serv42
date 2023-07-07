#include "../includes/serv.hpp"

void	serv::user(string b, User &user)
{
	size_t k = b.find_first_of(":");
	string s = b.substr(0, k);
	stringstream ss(s);
	if (k == string::npos)
	{
		msg_err.ERR_NEEDMOREPARAMS(user.getUserFD(), "", "User");
		return;
	}
	if (k != b.size())
	{
		b = b.substr(k, b.size());
	}
	else
		b = "";
	string	tok;
	int i = 0;
	while (ss >> tok)
	{
		if (tok == "")
		{
			msg_err.ERR_NEEDMOREPARAMS(user.getUserFD(), "", "User");
			break ;
		}
		else
		{
			if (tok.size() > MaxLenght)
			{
				send(user.getUserFD(), "too long parameter\n", 20, 0);
				break;
			}
			switch (i)
			{
				case 1:
					user.setUserName(tok);
					break;
				case 2:
					user.setHostName(tok);
					break;
				case 3:
					user.setServName(tok);
					break;
			}
		}
		if(i > 4)
		{
			// msg_err.ERR_NEEDMOREPARAMS(getNewServerFD(), "", "User");
			send(user.getUserFD(), ":@localhost User :too many parametrs\n", 38, 0);
			break;
		}
		i++;
	}
	user.setRealName(b);
	if(user.getHostName() == "" || user.getUserName() == ""
		|| user.getRealName() == "" || user.getServName() == "")
	{
		tok = "";
		user.setUserName(tok);
		user.setHostName(tok);
		user.setServName(tok);
		msg_err.ERR_NEEDMOREPARAMS(user.getUserFD(), "", "User");
		return;
	}
	if (user.getNickName() != "" && user.getPassFlag())
	{
		send(user.getUserFD(), "USER :You are registered\n", 26, 0);
		user.functionality = true;
	}
}

void	serv::pass(string b, User &user)
{
	stringstream ss(b);
	string p;
	ss >> p;

	cout << "client's input = ["<< p << "]"<< endl;
	cout << "the server password = ["<< password << "]"<< endl;
	if (p == password)
	{
		cout << "the client loged in" << endl;
		if (!user.getPassFlag())
			user.changePassFlag();
		if (user.getNickName() != "" && user.getHostName() != "")
		{	
			send(user.getUserFD(), "PASS :You are registered\n", 26, 0);
			user.functionality = true;
		}
	}
	else
		send(user.getUserFD(), "PASS :Wrong password\n", 22, 0);
	ss.clear();
}

void	serv::ping(string b, User &use)
{
	if (use.getPassFlag())
	{
		std::vector<string> arr = split(b);
		if (arr.size() > 2)
		{
			// ERR_NOSUCHSERVER
			send(use.getUserFD(), "PING :wrong input\n", 19,0);
			return;
		}
		if (arr.empty() || (!arr.empty() && arr[0].empty()))
		{
			// ERR_NOORIGIN()
			send(use.getUserFD(), "PING :No origin specified\n", 27,0);
			return;
		}
		else
		{
			int fd = findUserByNick(arr[0]);
			if (fd)
			{
				cout << "PING message got from nick " << use.getNickName() << endl;
				pong(arr[0], use);
				// send(use.getUserFD(), a.c_str(), 8 + b.size(), 0);
			}
			else
			{
				// ERR_NOSUCHSERVER
				send(use.getUserFD(), "PING :wrong inputt\n", 19,0);
				return;
			}
		}
	}
}

void	serv::pong(string b, User &user)
{
	std::vector<string> arr = split(b);

	if (arr.size() > 2)
	{
		// ERR_NOSUCHSERVER
		send(user.getUserFD(), "PONG :wrong input\n", 19,0);
		return;
	}
	if (arr.empty() || (!arr.empty() && arr[0].empty()))
	{
		// ERR_NOORIGIN()
		send(user.getUserFD(), "PONG :No origin specified\n", 27,0);
		return;
	}
	send(user.getUserFD(), ("PONG :message sent to nick " + arr[0] + "\n").c_str(), arr[0].size() + 28, 0);
}

void	serv::nick(string b, User &user)
{
	std::vector<string> arr = split(b);

	if (arr.size() > 2 || arr[0].find_first_of(",@!:") != string::npos || arr[0].size() > MaxLenght)
	{
		// ERR_ERRONEUSNICKNAME
		send(user.getUserFD(), "NICK :Erroneus nickname\n", 25,0);
		return;
	}
	for (size_t i = 0; i != arr[0].size(); i++)
		if (!isprint(arr[0][i]))
		{
			// ERR_ERRONEUSNICKNAME
			send(user.getUserFD(), "NICK :Erroneus nickname\n", 25,0);
			return;	
		}
	if (arr.empty() || (!arr.empty() && arr[0].empty()))
	{
		// ERR_NONICKNAMEGIVEN
		send(user.getUserFD(), "NICK :No nickname given\n", 25,0);
		return;
	}
	if (findUserByNick(arr[0]))
	{
		send(user.getUserFD(), "NICK :nickname collision\n", 26,0);
		// ERR_NICKCOLLISION
		return;
	}
	user.setNickName(arr[0]);
	if(!user.getHostName().empty() && user.getPassFlag())
	{
		send(user.getUserFD(), "NICK :You are registered\n", 26, 0);
		user.functionality = true;
	}
}


void	serv::cap(string b, User &user)
{
	b = "";
	cout<< "cap called by fd "<< user.getUserFD() << endl;
}

void	serv::ls(string b, User &user)
{
	b = "";
	cout << "ls called by fd "<< user.getUserFD() << endl;
}

void	serv::quit(string b, User &user)
{
	string a = "QUIT :";

	cout << user.getUserFD() << endl;
	if (b.empty())
		send(user.getUserFD(), (a + user.getNickName()).c_str(), (7 + user.getNickName().size()), 0);
	else
		send(user.getUserFD(), user.getNickName().c_str(), user.getNickName().size(), 0);
	getpeername(user.getUserFD(), (struct sockaddr*)(&def), (socklen_t*)&addrlen);
	FD_CLR(user.getUserFD(), &def);
	close(user.getUserFD());
	users.erase(user.getUserFD());
}

void	serv::privmsg(string b, User &user)
{
	stringstream ss(b);
	string tok;
	string msg;
	vector<string> rec;
	string tok2;

	tok2 = b.substr(tok.size(), b.size());
	if (tok.empty() || tok.find(":") != string::npos || !findUserByNick(tok))
	{
		// ERR_NORECIPIENT
		cout << "no client specified" << endl;
		return ;
	}
	else if (!tok.empty() && tok2.empty())
	{
		// ERR_NOTEXTTOSEND
		cout << "no text to send" << endl;
		return ;
	}
	user.getHostName();
	// else if ()
}


void	serv::join(string b, User &user)
{
	std::vector<string> arr = split(b);

	if (arr.empty() || (!arr.empty() && arr[0].empty()))
	{
		// ERR_NEEDMOREPARAMS
		send(user.getUserFD(), "JOIN :Need more params\n", 24, 0);
		return ;
	}
	map<std::string, Channel>::iterator i = user.getChannels().find(arr[0]);
	if (i != user.getChannels().end())
	{
		joinChannel(user, i->second, arr, false);
	}
	else
	{
		Channel chan(arr[0]);
		chan.oper.insert(std::pair<std::string, bool> (user.getNickName(), true));
		joinChannel(user, chan, arr, true);
	}
}

void	serv::kick(std::string b, User &user)
{
	std::vector<std::string> arr = split(b);

	if (arr.empty() || (!arr.empty() && arr[0].empty()) || (!arr[0].empty() && arr[1].empty()))
	{
		// ERR_NEEDMOREPARAMS
		send(user.getUserFD(), "KICK :Need more params\n", 24, 0);
		return ;
	}
	else if (arr.size() > 2)
	{
		send(user.getUserFD(), "KICK :Too many parameters\n", 27, 0);
		return ;
	}

	std::map<std::string, Channel>::iterator i = user.getChannels().find(arr[0]);
	if (i->second.oper.find(user.getNickName()) != i->second.oper.end())
	{
		if (i == user.getChannels().end())
		{
			// ERR_NOSUCHCHANNEL
			send(user.getUserFD(), "KICK :No such channel\n", 23, 0);
			return ;
		}
		std::map<int, User>::iterator it = i->second.getMembers().find(findUserByNick(arr[0]));
		if (it == i->second.getMembers().end())
		{
			// ERR_NOTONCHANNEL
			send(user.getUserFD(), "KICK :No such user\n", 20, 0);
			return ;
		}
		sendAll(i->second.getMembers(), "KICK", "kick " + it->second.getNickName() + " from " + i->second.getChannelName());
		i->second.getMembers().erase(it);
	}
	else
	{
		// ERR_CHANOPRIVSNEEDED
		send(user.getUserFD(), "KICK :You're not channel operator\n", 35, 0);
		return ;
	}
}

void	serv::invite(std::string b, User &user)
{
	std::vector<std::string> arr = split(b);
	if (arr.empty() || (!arr.empty() && arr[0].empty()) || (!arr[0].empty() && arr[1].empty()))
	{
		// ERR_NEEDMOREPARAMS
		send(user.getUserFD(), "INVITE :Need more params\n", 26, 0);
		return ;
	}
	else if (arr.size() > 2)
	{
		send(user.getUserFD(), "INVITE :Too many parameters\n", 29, 0);
		return ;
	}
	std::map<std::string, Channel>::iterator ii = user.getChannels().find(arr[1]);
	if (ii == user.getChannels().end())
	{
		// ERR_NOTONCHANNEL
		send(user.getUserFD(),  (arr[1] + " :You're not on that channel\n").c_str(), 36 + arr[1].size(), 0);
		return ;
	}
	std::map<int, User>::iterator it = ii->second.getMembers().find(findUserByNick(arr[0]));
	if (it != ii->second.getMembers().end())
	{
		// ERR_USERONCHANNEL
		send(user.getUserFD(),  (arr[0] + " " + arr[1] + " :You're not on that channel\n").c_str(), 37 + arr[1].size() + arr[0].size(), 0); 
		return ;
	}
	if (!findUserByNick(arr[0]))
	{
		// ERR_NOSUCHNICK
		send(user.getUserFD(),  (arr[0] + " :You're not on that channel\n").c_str(), 36 + arr[0].size(), 0); 
		return ;
	}
	if (ii->second.i && ii->second.oper.find(user.getNickName()) == ii->second.oper.end())
	{
		// ERR_CHANOPRIVSNEEDED
		send(user.getUserFD(),  (arr[1] + " :You're not on that channel's oper\n").c_str(), 43 + arr[1].size(), 0); 
		return ;
	}
	ii->second.setMembers(it->second.getUserFD(), it->second);
	it->second.setChannels(ii->second.getChannelName(), ii->second);
	sendAll(ii->second.getMembers(), "INVITE", "the " + arr[0] + " entered to the channel"); 
}

void	serv::topic(std::string b, User &user)
{
	std::vector<std::string> arr = split(b);
	if (arr.size() == 0)
	{
		// ERR_NEEDMOREPARAMS
		send(user.getUserFD(), "TOPIC :Need more params\n", 26, 0);
		return ;
	}
	std::map<std::string, Channel>::iterator it = user.getChannels().find(arr[0]);
	if (it == user.getChannels().end())
	{
		// ERR_NOTONCHANNEL
		send(user.getUserFD(),  (arr[1] + " :You're not on that channel\n").c_str(), 36 + arr[0].size(), 0);
		return ;
	}
	if (arr.size() == 1)
	{
		if (it->second.getChannelTopic().empty())
		{
			send(user.getUserFD(), (it->second.getChannelName() + " :No topic is set\n").c_str(), 19 + it->second.getChannelName().size(), 0);
			return ;
		}
		send(user.getUserFD(), (it->second.getChannelName() + " :" + it->second.getChannelTopic() + "\n").c_str(), 5 + it->second.getChannelName().size() + it->second.getChannelTopic().size(), 0);
	}
	else
	{
		if (arr[1][0] != ':')
		{
			send(user.getUserFD(), "TOPIC :Wrong argument syntax\n", 30, 0);
			return ;
		}
		std::string t = b.substr(b.find(':') + 1);
		if (it->second.oper.find(user.getNickName()) == it->second.oper.end())
		{
			// ERR_CHANOPRIVSNEEDED
			send(user.getUserFD(),  (arr[1] + " :You're not on that channel's oper\n").c_str(), 43 + arr[1].size(), 0); 
			return ;
		}
		it->second.setChannelTopic(t);
	}

}


void	serv::mode(string b, User &user)
{
	std::vector<std::string> arr = split(b);
	if (arr.empty() || (!arr.empty() && arr[0].empty()) || (!arr[0].empty() && arr[1].empty()))
	{
		// ERR_NEEDMOREPARAMS
		send(user.getUserFD(), "MODE :Need more params\n", 24, 0);
		return ;
	}
	if (arr.size() > 2)
	{
		send(user.getUserFD(), "MODE :Too many params\n", 23, 0);
		return ;
	}
	std::map<std::string, Channel>::iterator it = user.getChannels().find(arr[0]);
	if (it == user.getChannels().end())
	{
		// ERR_NOTONCHANNEL
		send(user.getUserFD(),  (arr[1] + " :You're not on that channel\n").c_str(), 36 + arr[0].size(), 0);
		return ;
	}
	if ((it->second.oper.find(user.getNickName())) == it->second.oper.end())
	{
		// ERR_CHANOPRIVSNEEDED
		send(user.getUserFD(), "MODE :You're not channel operator\n", 35, 0);
		return ;
	}
	
}


