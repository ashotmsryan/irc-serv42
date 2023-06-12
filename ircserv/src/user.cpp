#include "../includes/user.hpp"

User::User(int f){fd = f; pass = false;}

void	User::setHostName(std::string &n){hostName = n;}
void	User::setUserName(std::string &n){uName = n;}
void	User::setRealName(std::string &n){realName = n;}
void	User::setServName(std::string &n){servName = n;}
void	User::changePassFlag(){!pass ? pass = true : pass = false;}
void	User::setUserFD(int f){fd = f;}

std::string &User::getHostName(){return(hostName);}
std::string &User::getUserName(){return(uName);}
std::string &User::getRealName(){return(realName);}
std::string &User::getServName(){return(servName);}
bool		 User::getPassFlag(){return(pass);}
int 		&User::getUserFD(){return(fd);}

