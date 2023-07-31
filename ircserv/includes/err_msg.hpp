#include <iostream>
#include <iostream>
#include <sys/socket.h>

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
	// USER
	// PASS
	// NICK

	void	RPL_PONG(int cl_fd, std::string nick);
	void	RPL_QUIT(int cl_fd, std::string nick, std::string msg);
	void	RPL_INVITE(int cl_fd, std::string cname, std::string nick);
	void	RPL_TOPIC(int cl_fd, std::string cname, bool flag, std::string topic);
	
	void	RPL_NAMREPLY(int cl_fd, std::string cname, std::string topic);
	void	RPL_JOIN(int cl_fd, std::string cname);
	void	RPL_REGISTER(int cl_fd, std::string nick);


	// PRIVMSG
	// JOIN
	// KICK
	// MODE
};