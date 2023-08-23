#ifndef DATA_HPP
# define DATA_HPP

#include <map>
#include <vector>
#include <array>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sstream>
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

	bool	fromJoinCheck;
	struct sockaddr_in addr;
	int addrlen;
	err_msg msg_err;
	//////////////--setter--/////////////////
	void	setPassword(string pass);
	bool	setPort(string port);
	////////////////////////////////////////////////
	vector<std::string> split(std::string &str);
};


#endif
