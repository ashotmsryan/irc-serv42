#include "../includes/serv.hpp"

serv::serv()
{
	cmd["USER"] = &serv::user;
	cmd["PASS"] = &serv::pass;
	// cmd["PICK"] = &serv::pick;
	// cmd["PING"] = &serv::ping;
	// cmd["PONG"] = &serv::pong;
	// cmd["QUIT"] = &serv::quit;
	// cmd["ERROR"] = &serv::error;
	// cmd["AUTHENTICATE"] = &serv::authenticate;
	// cmd["CAP"] = &serv::cap;
}

int	serv::maxFD()
{
	int max = 3;

	for(map<int, User>::iterator i = users.begin(); i != users.end(); i++)
	{
		if (i->second.getUserFD() > max)
			max = i->second.getUserFD();
	}
	return(max + 1);
}


bool	serv::startServ()
{
	int opt = 1;
	serv_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_fd == 0)
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
		// fcntl(getServerFD(),F_SETFL,O_NONBLOCK);
	int addrlen = sizeof(addr);
	int select_flag;
	fd_set	def;
	//clear the socket set
	def = readFD;
	timeval t;
	t.tv_usec = 300000;
	t.tv_sec = 0;
	// if (fcntl(getServerFD(), F_SETFL, O_NONBLOCK) < 0)
	// 	perror("fcntl");
	cout << "ServerFD = "<< serv_fd << endl;
	FD_ZERO(&readFD);
	while (true)
	{
		readFD = def;
		//add server socket to set 
		FD_SET(serv_fd, &readFD);
 		//wait for an activity on one of the sockets , timeout is NULL
        //so wait indefinitely 
		cout << "maxFD = " << maxFD() <<endl;
		max_sd = std::max(serv_fd, maxFD());
		select_flag = select(max_sd + 1, &readFD, &def, NULL, NULL);
		// cout << "hasav" << endl;
		if (select_flag < 0 && errno != EINTR)
		{
			std::cout << "select :Something went wrong" << std::endl;
			continue;
		}
		if (FD_ISSET(serv_fd, &readFD))
		{
			// cout << "tee ba hasa" << endl;
			User	new_user(0);
			//to accept a client connection request on a listening socket
			new_user.setUserFD(accept(serv_fd, (sockaddr*)(&addr), (socklen_t *)(&addrlen)));
			if (new_user.getUserFD() < 0)
			{
				perror("accept");
				return true;
			}
			FD_SET(new_user.getUserFD(), &readFD);
			users.insert(pair<int, User> (new_user.getUserFD(), new_user));
			std::cout << "New client added" << std::endl;
		}
		char buf[1000] = {0};
		string e;
		for (map<int, User>::iterator i = users.begin(); i != users.end(); i++)
		{
			cout << i->first << endl;
			if(read(i->first, buf, 1000) < 0)
			{
				getpeername(i->first, (struct sockaddr*)(&readFD), (socklen_t*)&addrlen);  
				perror("read:");
               	cout << "Host disconnected , ip " << inet_ntoa(addr.sin_addr) << ", port " << port << endl;
				close(i->first);
				users.erase(i);
			}
			stringstream ss(buf);
			ss >> e;
			map<string, void(serv::*)(string, User&)>::iterator it = cmd.find(e);
				cout << e << std::endl;
			if(it == cmd.end())
			{
				msg_err.ERR_UNKNOWNCOMMAND(i->first, e);
			}
			else
			{
				if (i->second.getPassFlag() || (!i->second.getPassFlag() && it->first == "PASS"))
					(this->*(it->second))(buf, i->second);
				else
					msg_err.ERR_NOTREGISTERED(i->first, e);
			}
			// int j = -1;
			// while (j != 1000)
			// 	buf[++j] = 0;
		}
			cout << "aaaaaaaa" << endl;
	}
	return (false);
}