#pragma once

#include "Server.hpp"
#include <sys/types.h>
#include <fstream>
#include <time.h>
#include <sstream>

std::vector<std::string>	ft_split(const std::string &str, const std::string &charset);
Server						*parse_server(const std::vector<std::string> config, size_t *line_count);
Location					parse_location(const std::vector<std::string> &config, size_t *line_count, Server *server);
std::string					get_time_stamp(void);
void						error_handle(std::string msg);

class   Webserv
{
public:
	int						high_fd;
	fd_set					listen_set;
	fd_set					lcopy_set;
	fd_set					write_set;
	fd_set					wcopy_set;
	std::list<Server*>		_servers;
	
	void					parse_config(char *file_name);
	void					build();
	void					listen();
	Webserv(char *file);
	~Webserv();

};
