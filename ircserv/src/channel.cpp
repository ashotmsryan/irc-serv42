#include "../includes/channel.hpp"

std::map<int, User> &Channel::getChannels(){return (members);}

void	Channel::setChannels(int fd, User &user){members.insert(pair<int, User> (fd, user));}

