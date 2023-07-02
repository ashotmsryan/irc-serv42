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

#define MaxLenght 20

class Data : public err_msg
{
	bool	checkPort(string &port);
public:
	Data();
	int		port;
	string	password;

	int		serv_fd;
	int		max_sd;
	fd_set	def;

	struct sockaddr_in addr;
	int addrlen;
	err_msg msg_err;
	//////////////--setter--/////////////////
	void	setPassword(string pass);
	bool	setPort(string port);
	////////////////////////////////////////////////

};
