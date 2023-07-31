#include "../includes/err_msg.hpp"

// ####################################### ERROR ########################################


void	err_msg::ERR_UNKNOWNCOMMAND(int cl_fd, std::string b)
{
	send(cl_fd, (":" + b + "@localhost 421 Unknown command\n").c_str(), (b.size() + 34),0);
}

void	err_msg::ERR_NEEDMOREPARAMS(int cl_fd, std::string cmd)
{
	send(cl_fd, (":" + cmd + "@localhost 461 Not enough parameters\n").c_str(), (cmd.size() + 39), 0);
}

void	err_msg::ERR_NOTREGISTERED(int cl_fd, std::string nick)
{
	send(cl_fd, (":" + nick + "@localhost 451 You have not registered\n").c_str(), (nick.size() + 42), 0);
}

void	err_msg::ERR_ALREADYREGISTRED(int cl_fd, std::string nick)
{
	std::cout << "already registerd\n";
	send(cl_fd, (":" + nick + "@localhost 462 You may not reregister\n").c_str(), (nick.size() + 41), 0);
}

void	err_msg::ERR_ERRONEUSNICKNAME(int cl_fd, std::string nick)
{
	send(cl_fd, (":" + nick + "@localhost 432 Erroneus nickname\n").c_str(), (nick.size() + 36), 0);
}

void	err_msg::ERR_NONICKNAMEGIVEN(int cl_fd, std::string nick)
{
	send(cl_fd, (":" + nick + "@localhost 431 No nickname given\n").c_str(), (nick.size() + 36), 0);
}

void	err_msg::ERR_NICKCOLLISION(int cl_fd, std::string nick)
{
	send(cl_fd, (":" + nick + "@localhost 436 Nickname collision KILL\n").c_str(), (nick.size() + 42), 0);
}

void	err_msg::ERR_NICKNAMEINUSE(int cl_fd, std::string nick)
{
	send(cl_fd, (":" + nick + "@localhost 433 Nickname is already in use\n").c_str(), (nick.size() + 42), 0);
}

void	err_msg::ERR_NOORIGIN(int cl_fd, std::string cmd)
{
	send(cl_fd, (":" + cmd + "@localhost 409 No origin specified\n").c_str(), (cmd.size() + 37), 0);
}

void	err_msg::ERR_NOTEXTTOSEND(int cl_fd, std::string cmd)
{
	send(cl_fd, (":" + cmd + "@localhost 412 No text to send\n").c_str(), (cmd.size() + 34), 0);
}

void	err_msg::ERR_NORECIPIENT(int cl_fd, std::string cmd)
{
	send(cl_fd, (":@localhost 411 No recipient given " + cmd + "\n").c_str(), (cmd.size() + 38), 0);
}

void	err_msg::ERR_TOOMANYTARGETS(int cl_fd, std::string tg)
{
	send(cl_fd, (":" + tg + "@localhost 407 Duplicate recipients. No message \\n").c_str(), (tg.size() + 53), 0);
}

void	err_msg::ERR_NOSUCHNICK(int cl_fd, std::string nick)
{
	send(cl_fd, (":" + nick + "@localhost 401 No such nick/channel\n").c_str(), (nick.size() + 39), 0);
}

void	err_msg::ERR_CANNOTSENDTOCHAN(int cl_fd, std::string cname)
{
	send(cl_fd, (":" + cname + "@localhost 404 Cannot send to channel\n").c_str(), (cname.size() + 41), 0);
}

void	err_msg::ERR_INVITEONLYCHAN(int cl_fd, std::string cname)
{
	send(cl_fd, (":" + cname + "@localhost 473 Cannot join channel (+i)\n").c_str(), (cname.size() + 42), 0);
}

void	err_msg::ERR_NOSUCHCHANNEL(int cl_fd, std::string cname)
{
	send(cl_fd, (":" + cname + "@localhost 403 No such channel\n").c_str(), (cname.size() + 34), 0);
}

void	err_msg::ERR_CHANOPRIVSNEEDED(int cl_fd, std::string cname)
{
	send(cl_fd, (":" + cname + "@localhost 482 You're not channel operator\n").c_str(), (cname.size() + 46), 0);
}

void	err_msg::ERR_NOTONCHANNEL(int cl_fd, std::string cname)
{
	send(cl_fd, (":" + cname + "@localhost 442 You're not on that channel\n").c_str(), (cname.size() + 45), 0);
}

void	err_msg::ERR_USERONCHANNEL(int cl_fd, std::string cname, std::string nick)
{
	send(cl_fd, (":" + nick + cname + "@localhost 443 is already on channel\n").c_str(), (cname.size() + nick.size() + 41), 0);
}

void	err_msg::ERR_UNKNOWNMODE(int cl_fd, std::string c)
{
	send(cl_fd, (":" + c + "@localhost 472 is unknown mode char to me\n").c_str(), (c.size() + 45), 0);
}

void	err_msg::ERR_BANNEDFROMCHAN(int cl_fd, std::string cname)
{
	send(cl_fd, (":" + cname + "@localhost 474 Cannot join channel (+b)\n").c_str(), (cname.size() + 43), 0);
}

void	err_msg::ERR_CHANNELISFULL(int cl_fd, std::string cname)
{
	send(cl_fd, (":" + cname + "@localhost 471 Cannot join channel (+l)\n").c_str(), (cname.size() + 43), 0);
}

void	err_msg::ERR_BADCHANNELKEY(int cl_fd, std::string cname)
{
	send(cl_fd, (":" + cname + "@localhost 475 Cannot join channel (+k)\n").c_str(), (cname.size() + 43), 0);
}

// ##################################### REPLY ##########################################

void	err_msg::RPL_PONG(int cl_fd, std::string nick)
{
	send(cl_fd, (":PONG@localhost PONG message from " + nick + "\n").c_str(), 37 + nick.size(), 0);
}

void	err_msg::RPL_QUIT(int cl_fd, std::string nick, std::string msg)
{
	send(cl_fd, (":QUIT@localhost " + nick + " " + msg + "\n").c_str(), 21 + nick.size() + msg.size(), 0);
}

void	err_msg::RPL_INVITE(int cl_fd, std::string cname, std::string nick)
{
	send(cl_fd, (":INVITE@localhost " + cname + " " + nick + "\n").c_str(), 21 + nick.size() + cname.size(), 0);
}

void	err_msg::RPL_TOPIC(int cl_fd, std::string cname, bool flag, std::string topic)
{
	if (flag)
		send(cl_fd, (":" + cname + "@localhost 332 No topic is set\n").c_str(), 34 + cname.size(), 0);
	else
		send(cl_fd, (":" + cname + "@localhost 332" + topic + "\n").c_str(), 19 + cname.size() + topic.size(), 0);
}

void	err_msg::RPL_NAMREPLY(int cl_fd, std::string cname, std::string members)
{
	send(cl_fd, (":" + cname + "@localhost 353" + members + "\n").c_str(), 19 + cname.size() + members.size(), 0);
}

void	err_msg::RPL_JOIN(int cl_fd, std::string cname)
{
	send(cl_fd, (":JOIN@localhost you joind the channel " + cname + "\n").c_str(), cname.size() + 41, 0);
}

void	err_msg::RPL_REGISTER(int cl_fd, std::string nick)
{
	send(cl_fd, (":" + nick + "@localhost 001 Welcome to the IRC Network, " + nick +"!\n").c_str(), 49 + (2*nick.size()), 0);
}