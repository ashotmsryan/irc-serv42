#pragma once

#include <iostream>

class User
{
	int			fd;
	std::string hostName;
	std::string uName;
	std::string realName;
	std::string servName;
	std::string nickName;

	bool 		pass;
	bool		oper;
	// map<>
public:
	bool functionality;
	User(int f);
	void	setHostName(std::string &n);
	void	setNickName(std::string &n);
	void	setUserName(std::string &n);	
	void	setRealName(std::string &n);
	void	setServName(std::string &n);
	void	setUserFD(int f);

	void	changePassFlag();

	std::string &getHostName();
	std::string &getUserName();
	std::string &getRealName();
	std::string &getServName();
	std::string getNickName();
	bool 		getPassFlag();
	int 		&getUserFD();
};