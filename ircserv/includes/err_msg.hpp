#ifndef  ERR_MSG_HPP
# define ERR_MSG_HPP

# include <iostream>
# include <sys/socket.h>

class err_msg
{
public:
// ############################ ERROR #################################
	void	ERR_UNKNOWNCOMMAND(int cl_fd, std::string cmd);
	void	ERR_NEEDMOREPARAMS(int cl_fd, std::string cmd);
	void	ERR_NOTREGISTERED(int cl_fd, std::string cmd);
	void	ERR_ALREADYREGISTRED(int cl_fd, std::string nick);
	void	ERR_ERRONEUSNICKNAME(int cl_fd, std::string nick);
	void	ERR_NONICKNAMEGIVEN(int cl_fd, std::string nick);
	void	ERR_NICKCOLLISION(int cl_fd, std::string nick);
	void	ERR_NICKNAMEINUSE(int cl_fd, std::string nick);
	void	ERR_NOORIGIN(int cl_fd, std::string cmd);
	void	ERR_NOTEXTTOSEND(int cl_fd, std::string cmd);
	void	ERR_NORECIPIENT(int cl_fd, std::string cmd);
	void	ERR_TOOMANYTARGETS(int cl_fd, std::string tg);
	void	ERR_NOSUCHNICK(int cl_fd, std::string nick);
	void	ERR_CANNOTSENDTOCHAN(int cl_fd, std::string cname);
	void	ERR_INVITEONLYCHAN(int cl_fd, std::string cname);
	void	ERR_NOSUCHCHANNEL(int cl_fd, std::string cname);
	void	ERR_CHANOPRIVSNEEDED(int cl_fd, std::string cname);
	void	ERR_NOTONCHANNEL(int cl_fd, std::string cname);
	void	ERR_USERONCHANNEL(int cl_fd, std::string cname, std::string nick);
	void	ERR_UNKNOWNMODE(int cl_fd, std::string c);
	void	ERR_BANNEDFROMCHAN(int cl_fd, std::string cname);
	void	ERR_CHANNELISFULL(int cl_fd, std::string cname);
	void	ERR_BADCHANNELKEY(int cl_fd, std::string cname);

// ############################ REPLY #################################

	void	RPL_PONG(int cl_fd, std::string nick);
	void	RPL_KICK(int cl_fd, std::string nick, std::string uname, std::string tname, std::string cname);
	void	RPL_QUIT(int cl_fd, std::string nick, std::string msg);
	void	RPL_INVITING(int cl_fd, std::string cname, std::string nick, std::string uname, std::string tname);
	void	RPL_TOPIC(int cl_fd, std::string cname, bool flag, std::string topic);
	
	void	RPL_REGISTER(int cl_fd, std::string nick, std::string uname);
	void	RPL_WHOREPLY(int cl_fd, std::string nick, std::string cname, std::string memb, std::string username);
	void	RPL_NAMREPLY(int cl_fd, std::string nick, std::string cname, std::string members);
	void	RPL_ENDOFNAMES(int cl_fd, std::string nick, std::string cname);
	void	RPL_ENDOFWHO(int cl_fd, std::string nick, std::string cname);
	void	RPL_PRIVMSG(int cl_fd, std::string nick, std::string uname, std::string tname, std::string msg);
	void	RPL_NOTICE(int cl_fd, std::string nick, std::string tname, std::string msg);
	void	RPL_PART(int cl_fd, std::string nick, std::string uname, std::string cname);

};
#endif 