#include "../includes/err_msg.hpp"

void	err_msg::ERR_UNKNOWNCOMMAND(int cl_fd, std::string b)
{
	std::string a = " :Unknown command\n";
	b += a;
	send(cl_fd, (":@localhost " + b).c_str(), (b.size()+12),0);
}

void	err_msg::ERR_NEEDMOREPARAMS(int cl_fd, std::string clName, std::string cmd)
{
	send(cl_fd, (clName + ":@localhost " + cmd + " :Not enough parameters").c_str(), (cmd.size() + clName.size()+37),0);
}

void	err_msg::ERR_NOTREGISTERED(int cl_fd, std::string cmd)
{
	send(cl_fd, (":@localhost " + cmd + " :You have not registered\n").c_str(), (cmd.size() +40),0);
}