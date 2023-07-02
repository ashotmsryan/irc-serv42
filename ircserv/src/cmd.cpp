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
		stringstream ss(b);
		string token;
		string token2;
		ss >> token;
		ss >> token2;
		if (!token2.empty())
		{
			// ERR_NOSUCHSERVER
			send(use.getUserFD(), "PING :wrong input\n", 19,0);
			return;
		}
		if (token == "")
		{
			// ERR_NOORIGIN()
			send(use.getUserFD(), "PING :No origin specified\n", 27,0);
			return;
		}
		else
		{
			int fd = findUserByNick(token);
			if (fd)
			{
				cout << "PING message got from nick " << use.getNickName() << endl;
				pong(token, use);
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
	stringstream ss(b);
	string tok;
	string tok2;
	
	ss >> tok;
	ss >> tok2;
	if (!tok2.empty())
	{
		// ERR_NOSUCHSERVER
		send(user.getUserFD(), "PONG :wrong input\n", 19,0);
		return;
	}
	if (tok.empty())
	{
		// ERR_NOORIGIN()
		send(user.getUserFD(), "PONG :No origin specified\n", 27,0);
		return;
	}
	send(user.getUserFD(), ("PONG :message sent to nick " + tok + "\n").c_str(), tok.size() + 28, 0);
}

void	serv::nick(string b, User &user)
{
	stringstream ss(b);
	string token;
	string token2;

	ss >> token;
	ss >> token2;
	if (!token2.empty() || token.find_first_of(",@!:") != string::npos || token.size() > MaxLenght)
	{
		// ERR_ERRONEUSNICKNAME
		send(user.getUserFD(), "NICK :Erroneus nickname\n", 25,0);
		return;
	}
	for (size_t i = 0; i != token.size(); i++)
		if (!isprint(token[i]))
		{
			// ERR_ERRONEUSNICKNAME
			send(user.getUserFD(), "NICK :Erroneus nickname\n", 25,0);
			return;	
		}
	if (token.empty())
	{
		// ERR_NONICKNAMEGIVEN
		send(user.getUserFD(), "NICK :No nickname given\n", 25,0);
		return;
	}
	if (findUserByNick(token))
	{
		send(user.getUserFD(), "NICK :nickname collision\n", 26,0);
		// ERR_NICKCOLLISION
		return;
	}
	user.setNickName(token);
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




// void	serv::authenticate(string b, User &new_user)
// {
// 	cout << "authenticate"<<b << std::endl;
// }
// void	serv::error(string b, User &new_user)
// {
// 	cout << "error" <<b<< std::endl;
// }

