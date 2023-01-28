#pragma once

#include <map>
#include <vector>
# include <sys/socket.h>
# include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
class Server;
#include "Location.hpp"



class Server
{
	public:
		std::list<Location>					locations;
		std::list<std::string>				server_names;
		std::map<std::string, std::string>  cgis;
		std::map<unsigned int, std::string> error_pages;
		std::string							root;
		std::string							index;
		std::string							rawIndex;
		std::string							ip_adress;
		unsigned int						port;
		int									listen_fd;
		int									status;
		unsigned long						max_client_body_size;
	public:
		Server();
		~Server();
		int		setup();
		bool	is_valid();
};
