#pragma once

#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include "cstring"
#include "Server.hpp"
#include "client.hpp"

class	handleCgi
{
	public:

		std::string	cgiRunner(std::map<int, std::string> _env, std::string cgiPath, Location *lit, Client *cli);
		std::string	cgiEnv(Location *lit, std::string cgiPath, Client *cli);
		char		**envToDoubleChar(std::map<int, std::string> &_env);
};