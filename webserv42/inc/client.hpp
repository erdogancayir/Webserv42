#pragma once
#include "Server.hpp"

class Client
{
	public:
		int									fd;
		int									multi;
		int									status;
		int									code;
		std::string							resp;
		std::string							method;
		std::string							dir;
		std::string							protocol;
		std::string							postVal;
		std::string							serverName;
		std::string							queryString;
		size_t								postLen;
		Server								*sv;
		std::map<std::string, std::string>	headers;

		void reqParser(char *req);
		Client(Server *sv, int fd);
};


