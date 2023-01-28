#pragma once

#include <iostream>
#include <list>
class Location;
#include "Server.hpp"
#include "webserv.hpp"

class Location
{
	private:
		
	public:
		Server					*server;
		std::string				path;
		std::list<std::string>	allow;
		unsigned int			HTTP_redirection_type;
		std::string				HTTP_redirection;
		std::string				location_root;
		std::string				index;
		std::string				rawIndex;
		Location();
		~Location();
};