#include "../includes/data.hpp"

Data::Data()
{
	serv_fd = 0;
	port = 0;
	fromJoinCheck = false;
}
// fd_set &Data::getFDset(){return(clientFD);}
// // void	Data::setFDset(int fd){}

// string	Data::getPassword() const {return (password);}

// int		Data::getServerFD() const {return(serv_fd);}
// void	Data::setServerFD(int fd) {serv_fd = fd;}

// struct sockaddr_in &Data::getSockAddr(){return (addr);}
// void	Data::setSockAddr(struct sockaddr_in s) {addr = s;}

void	Data::setPassword(string pass){password = pass;}

bool	Data::checkPort(string &port)
{
	for (size_t i = 0; i < port.size(); i++)
		if (!isdigit(port[i]))
			return (true);
	int i = atoi(port.c_str());
	std::stringstream ss;
	ss << i;
	std::string s = ss.str();
	i = -1;
	while(port[++i] && port[i] == '0');
	if (s != &port[i])
		return(true);
	return (false);
}

bool	Data::setPort(string p)
{
	if (checkPort(p))
		cerr << "port should be int type" << endl;
	else if (p.size() > 5)
		cerr << "port can have no more than 5 numbers!" << endl;
	else if (atoi(p.c_str()) > 65535 || atoi(p.c_str()) < 0)
		cerr << "port can not be more than 65535 and less than 0" << endl;
	else 
	{
		port = atoi(p.c_str());
		return (false);
	}
	return (true);
}

std::vector<std::string> Data::split(std::string &str)
{
	std::stringstream ss(str);
	vector<std::string> ret;
	std::string s = " ";

	if (str.empty())
		return (ret);
	while(ss >> s)
	{
		if (s.empty())
			return (ret);
		ret.push_back(s);
	}
	return (ret);
}
