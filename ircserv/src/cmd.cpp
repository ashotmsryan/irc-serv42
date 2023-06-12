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
			return;
		}
		else
		{
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
			return;
		}
		i++;
	}
	user.setRealName(b);
	if(user.getHostName() == "" || user.getUserName() == ""
		|| user.getRealName() == "" || user.getServName() == "")
	{
		msg_err.ERR_NEEDMOREPARAMS(user.getUserFD(), "", "User");
		return;
	}
}

void	serv::pass(string b, User &user)
{
	stringstream ss(b);
	string p;
	ss >> p;
	ss >> p;

	if (p == password)
	{
		send(user.getUserFD(), "PASS :You are registered\n", 26, 0);
		cout << "New client registered" << endl;
		user.changePassFlag();
		
	}
	else
		send(user.getUserFD(), "PASS :Wrong password\n", 22, 0);
	ss.clear();
}













// void	serv::pick(string b, User &new_user)
// {
// 	cout << "pick" << b<< std::endl;
// }


// void	serv::ping(string b, User &new_user)
// {
// 	cout << "ping" <<b << std::endl;
// }

// void	serv::pong(string b, User &new_user)
// {
// 	cout << "pong" <<b<< std::endl;
// }

// void	serv::quit(string b, User &new_user)
// {
// 	cout << "quit" <<b<< std::endl;
// }

// void	serv::error(string b, User &new_user)
// {
// 	cout << "error" <<b<< std::endl;
// }

// void	serv::authenticate(string b, User &new_user)
// {
// 	cout << "authenticate"<<b << std::endl;
// }

// void	serv::cap(string b, User new_u&ser)
// {
// 	cout << "cap"<<b << std::endl;
// }