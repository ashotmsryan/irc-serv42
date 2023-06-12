#include "../includes/serv.hpp"
#include "../includes/data.hpp"

int main(int argc, char **argv, char **env)
{
	(void)env;
	if (argc == 3)
	{
		serv	data;
		data.setPassword(argv[2]);
		if(data.setPort(argv[1]))
			return (1);
		if (data.startServ())
			return (1);
	}
	else
		cerr << "Wrong quantity of arguments-> TRY './ircserv <port> <password>'" << endl;
	return (0);
}