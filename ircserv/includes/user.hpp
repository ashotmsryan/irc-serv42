#ifndef USER_HPP
# define USER_HPP

#include "channel.hpp"
class Channel;


class User
{
	int			fd;
	bool 		pass;
	std::string hostName;
	std::string uName;
	std::string realName;
	std::string servName;
	std::string nickName;

	map<std::string, Channel> channels;
public:
	bool		functionality;
	User(int f);
	void	setHostName(std::string &n);
	void	setNickName(std::string &n);
	void	setUserName(std::string &n);	
	void	setRealName(std::string &n);
	void	setServName(std::string &n);
	void	setUserFD(int f);
	void	setChannels(std::string n, Channel &chan);

	void	changePassFlag();

	std::string &getHostName();
	std::string &getUserName();
	std::string &getRealName();
	std::string &getServName();
	std::string getNickName();
	bool 		getPassFlag();
	int 		&getUserFD();
	map<std::string, Channel> &getChannels();
};

#endif
