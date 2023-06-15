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
	FD_SET(new_user.getUserFD(), &def);
	users.insert(pair<int, User> (new_user.getUserFD(), new_user));
	std::cout << "New client added" << std::endl;	
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
	// if (fcntl(serv_fd, F_SETFL, O_NONBLOCK) < 0)
	// 	perror("fcntl");
	cout << "ServerFD = "<< serv_fd << endl;
	string str;
	string e;
	char buf[1000] = {0};
	while (true)
	{
		writeFD = readFD = def;
		// max_sd = std::max(serv_fd, maxFD());
		cout << "maxFD = " << maxFD() <<endl;
 		//wait for an activity on one of the sockets , timeout is NULL
        //so wait indefinitely 
		select_flag = select(maxFD() + 1, &readFD, &writeFD, NULL, NULL);
		if (select_flag < 0 && errno != EINTR)
		{
			std::cout << "select :Something went wrong" << std::endl;
			continue;
		}
		for (int fd = 0; fd <= maxFD(); fd++)
        {
            if (FD_ISSET(fd, &readFD))
            {
                if (fd == serv_fd)
                {
                    add_client();
                    break;
                }
                else
                {
            		if (read(fd, buf, 1000) < 0)
            		{
						getpeername(fd, (struct sockaddr*)(&readFD), (socklen_t*)&addrlen);  
						perror("read:");
						cout << "Host disconnected , ip " << inet_ntoa(addr.sin_addr) << ", port " << port << endl;
	        		    FD_CLR(fd, &def);
						close(fd);
						users.erase(fd);
            		    break;
            		}
		        	if (fd != serv_fd)
                	{
						stringstream ss(buf);
						ss >> e;
						map<string, void(serv::*)(string, User&)>::iterator it = cmd.find(e);
							cout << e << std::endl;
						if(it == cmd.end())
						{
							msg_err.ERR_UNKNOWNCOMMAND(fd, e);
						}
						else
						{
							if (users.find(fd)->second.getPassFlag() || (!users.find(fd)->second.getPassFlag() && it->first == "PASS"))
								(this->*(it->second))(str, users.find(fd)->second);
							else
								msg_err.ERR_NOTREGISTERED(users.find(fd)->first, e);
						}
                	}
                }
            }
        }
	}
	return (false);
}