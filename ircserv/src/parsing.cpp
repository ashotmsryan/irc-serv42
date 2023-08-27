#include "../includes/serv.hpp"

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

void	serv::deleteWhiteSpace(std::string &s)
{
	size_t i = -1;
	while (s[++i])
	{
		if (!isprint(s[i]))
			s.erase(i, 1);
	}
}

std::vector<std::string> serv::parsing(char *b, size_t len)
{
	size_t i = 0;
	size_t j = 0;
	std::string s;
	std::string buf;
	std::vector<std::string> ret;

	// PA\0SS 11\nNI\0CK aa\nU\0SER k k k k
	while(i < len)
	{
		if (b[i] || isprint(b[i]) || b[i] == '\n')
			buf.push_back(b[i]);
		i++;
	}
	i = 0;
	while(buf[i] || (!buf[i] && buf.size() != i))
	{
		if (buf[i] == '\n' || buf[i] == '\r')
		{
			s = buf.substr(j, i);
			deleteWhiteSpace(s);
			ret.push_back(s);
			s = "";
			j = i;
			while(buf[i] && (buf[i] == '\n' || buf[i] == '\r'))
				i++;
			j = i;
		}
		if (buf[i])
			i++;
	}
	if (buf[buf.size() - 1] != '\n' && buf[buf.size() - 1] != '\r')
	{
		ret.push_back(buf.substr(j, buf.size() - j));
		ret.back().append("\n");
	}
	i = 0;
	return (ret);
}