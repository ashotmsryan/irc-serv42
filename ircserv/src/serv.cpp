#include "../includes/serv.hpp"

serv::serv()
{
	cmd["USER"] = &serv::user;
	cmd["PASS"] = &serv::pass;
	cmd["NICK"] = &serv::nick;
	cmd["PING"] = &serv::ping;//chem imanum
	cmd["PONG"] = &serv::pong;//chem imanum
	cmd["QUIT"] = &serv::quit;
	cmd["PRIVMSG"] = &serv::privmsg;//kisat
	cmd["CAP"] = &serv::cap;
	// cmd["ERROR"] = &serv::error;
	// cmd["AUTHENTICATE"] = &serv::authenticate;
	
	cmd["JOIN"] = &serv::join;
	cmd["KICK"] = &serv::kick;
	cmd["INVITE"] = &serv::invite;
	cmd["TOPIC"] = &serv::topic;
	cmd["MODE"] = &serv::mode;
}

int	serv::findUserByNick (string nick)
{
	map<int, User>::iterator e = users.end();
	for (map<int, User>::iterator i = users.begin(); i != e; i++)
	{
		if (i->second.getNickName() == nick)
			return (i->first);
	}
	return (0);
}

int	serv::maxFD()
{
	int max = 3;

	for(map<int, User>::iterator i = users.begin(); i != users.end(); i++)
	{
		if (i->second.getUserFD() > max)
			max = i->second.getUserFD();
	}
	return(max);
}

void	serv::add_client()
{
	User	new_user(0);
	//to accept a client connection request on a listening socket
	new_user.setUserFD(accept(serv_fd, (sockaddr*)(&addr), (socklen_t *)(&addrlen)));
	if (new_user.getUserFD() < 0)
	{
		perror("accept");
		return ;
	}
	//add client soccet in the set
	FD_SET(new_user.getUserFD(), &def);
	//add client(user) to the map of users
	users.insert(pair<int, User> (new_user.getUserFD(), new_user));
	std::cout << "New client added" << std::endl;	
}


bool	serv::read_write(int fd)
{
	string e;
	string str;
	char buf[1000] = {0};

	if (recv(fd, buf, 1000, 0) < 0)
    {
		getpeername(fd, (struct sockaddr*)(&def), (socklen_t*)&addrlen);
		cout << "Host disconnected, ip " << inet_ntoa(addr.sin_addr) << ", port " << port << endl;
	    FD_CLR(fd, &def);
		users.erase(fd);
		if (fd != -1)
			close(fd);
        return true;
    }
	stringstream ss(buf);
	ss >> e;
	map<string, void(serv::*)(string, User&)>::iterator it = cmd.find(e);
	cout << "client [" << fd << "] = "  << e << std::endl;
	if(it == cmd.end())
	{
		msg_err.ERR_UNKNOWNCOMMAND(fd, e);
	}
	else
	{
		if (users.find(fd)->second.getPassFlag() || (!users.find(fd)->second.getPassFlag() && it->first == "PASS"))
		{
			str = buf;
			str = str.substr(e.size(), str.size() - e.size());

			if (users.find(fd)->second.functionality || (!users.find(fd)->second.functionality 
				&& ((it->first == "CAP") || (it->first == "USER")
				|| (it->first == "NICK") || (it->first == "PASS") || (it->first == "PING"))))
			{	
				(this->*(it->second))(str, users.find(fd)->second);
			}
			else
				msg_err.ERR_NOTREGISTERED(users.find(fd)->first, e);
		}
		else
			msg_err.ERR_NOTREGISTERED(users.find(fd)->first, e);
	}
	return (false);
}

bool	serv::startServ()
{
	int opt = 1;
	serv_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_fd < 0)
	{
		perror("faild socket!");
		return true;
	}
	//API level | enable reuse of local address | multiple sockets to bind to the same IP address and port
	if (setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		perror("setsockopt");
		return (true);
	}

	addr.sin_family = AF_INET;//IPv4
	addr.sin_addr.s_addr = INADDR_ANY; //0.0.0.0 socket can accept any connections on any available network interface of the system.
	addr.sin_port = htons(port);//convert the port number from host byte order to network byte order
	// to bind socket with specific newtwork address and port number on local muchine 
	if (bind(serv_fd,(const sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("bind");
		return true;
	}
	//number(55) of pending connections that socket can wait
	if (listen(serv_fd, 100) < 0)
	{
		perror("listen");
		return true;
	}
	addrlen = sizeof(addr);
	int select_flag;
	fd_set	readFD;
	fd_set	writeFD;

	//clear the socket set
	FD_ZERO(&def);
	//add server socket to set 
	FD_SET(serv_fd, &def);
	readFD = writeFD = def;
	timeval t;
	t.tv_usec = 300000;
	t.tv_sec = 0;
	cout << "ServerFD = "<< serv_fd << endl;
	while (true)
	{
		writeFD = readFD = def;
 		//wait for an activity on one of the sockets , timeout is NULL
        //so wait indefinitely 
		select_flag = select(maxFD() + 1, &readFD, &writeFD, NULL, NULL);
		if (select_flag < 0 && errno != EINTR)
		{
			std::cout << "select :Something went wrong" << std::endl;
			continue;
		}
		// cout << "maxFD = " << maxFD() << endl;
		for (int fd = 2; fd <= maxFD(); fd++)
        {
            if (FD_ISSET(fd, &readFD))
            {
                if (fd == serv_fd)
                {
                    add_client();
                    break;
                }
                else
					if (read_write(fd))
						break ;
            }
        }
	}
	return (false);
}