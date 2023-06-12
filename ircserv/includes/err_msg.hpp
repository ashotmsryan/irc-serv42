#include <iostream>
#include <iostream>
#include <sys/socket.h>

class err_msg
{
public:
	void	ERR_UNKNOWNCOMMAND(int cl_fd, std::string buf);
	void	ERR_NEEDMOREPARAMS(int cl_fd, std::string clName, std::string cmd);
	void	ERR_NOTREGISTERED(int cl_fd, std::string cmd);
};