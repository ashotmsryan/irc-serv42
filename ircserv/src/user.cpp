#include "../includes/user.hpp"

User::User(int f){fd = f; pass = false; functionality = false;}

void	User::setHostName(std::string &n){hostName = n;}
void	User::setUserName(std::string &n){uName = n;}
void	User::setRealName(std::string &n){realName = n;}
void	User::setServName(std::string &n){servName = n;}
void	User::setNickName(std::string &n){nickName = n;}
void	User::changePassFlag(){!pass ? pass = true : pass = false;}
void	User::setUserFD(int f){fd = f;}
void	User::setChannels(std::string n, Channel &chan){channels.insert(make_pair<std::string, Channel> (n, chan));}

std::string &User::getHostName(){return(hostName);}
std::string &User::getUserName(){return(uName);}
std::string &User::getRealName(){return(realName);}
std::string User::getNickName(){return(nickName);}
std::string &User::getServName(){return(servName);}
bool		 User::getPassFlag(){return(pass);}
int 		&User::getUserFD(){return(fd);}
map<std::string, Channel> &User::getChannels(){return (channels);}
