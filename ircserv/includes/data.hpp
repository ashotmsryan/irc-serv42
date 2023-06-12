#pragma once

#include <map>
#include <vector>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "err_msg.hpp"
using	namespace std;
class err_msg;

class Data : public err_msg
{
	bool	checkPort(string &port);
public:
	Data();
	int		serv_fd;
	int		port;
	string	password;

	fd_set	readFD;
	struct sockaddr_in addr;
	err_msg msg_err;
	int		max_sd;
	//////////////--setter--/////////////////
	void	setPassword(string pass);
	bool	setPort(string port);
	////////////////////////////////////////////////

};
