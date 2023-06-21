#pragma once

#include <fcntl.h>
#include "data.hpp"
#include <map>
#include <sstream>
#include "user.hpp"

class serv : public Data
{
	map<int, User> users;
	// map<string, User> users;
	map<string, void(serv::*)(string, User&)> cmd;

	bool	checkSockFD();
	int		maxFD();
	bool	checkCommand(char *buf);
	bool	read_write(int fd, fd_set &def, fd_set &readFD);
public:
	serv();
////////////////////---COMMANDS---//////////////////////
	void	user(string b, User &new_user);
	void	pass(string b, User &new_user);
	// void	pick(string b, User &new_user);
	void	ping(string b, User &new_user);
	void	pong(string b, User &new_user);
	// void	quit(string b, User &new_user);
	// void	error(string b, User &new_user);
	// void	authenticate(string b, User &new_user);
	// void	cap(string b, User &new_user);
//////////////////////////////////////////////////////
	
	bool	startServ();
	void	add_client();


	// template <typename T, typename A>
	// void	read_exec(T user, int addrlen, A)
	// {
	// 	char buf[1000] = {0};
	// 	string e;
	// 	for (A i = user.begin(); i != user.end(); i++)
	// 	{
	// 		if(read(i->second.getUserFD(), buf, 1000) < 0)
	// 		{
	// 			getpeername(i->second.getUserFD(), (struct sockaddr*)(&getSockAddr()), (socklen_t*)&addrlen);  
	//            	cout << "Host disconnected , ip " << inet_ntoa(getSockAddr().sin_addr) << ", port " << getPort() << endl;
	// 			close(i->second.getUserFD());
	// 			user.erase(i);
	// 		}
	// 		stringstream ss(buf);
	// 		ss >> e;
	// 		map<string, void(serv::*)(string, User&)>::iterator it = cmd.find(e);
	// 			cout << e << std::endl;
	// 		if(it == cmd.end())
	// 		{
	// 			msg_err.ERR_UNKNOWNCOMMAND(i->second.getUserFD(), e);
	// 		}
	// 		else
	// 		{
	// 			if (i->second.getPassFlag() || (!i->second.getPassFlag() && it->first == "PASS"))
	// 				(this->*(it->second))(buf, i->second);
	// 			else
	// 				msg_err.ERR_NOTREGISTERED(i->second.getUserFD(), e);
	// 		}
	// 		// int j = -1;
	// 		// while (j != 1000)
	// 		// 	buf[++j] = 0;
	// 	}
	// }
};

