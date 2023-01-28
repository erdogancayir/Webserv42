#include "../inc/webserv.hpp"

Webserv::Webserv(char *file)
{
	parse_config(file);
}

Webserv::~Webserv()
{
	_servers.clear();
}

std::string	read_a_file(std::fstream &file)
{
	std::string	res;
	std::string	tmp;

	while (getline(file, tmp))
		res += tmp + "\n";
	return (res);
}

void	Webserv::parse_config(char *file_name)
{
	std::fstream				file;
	std::string					str;
	std::vector<std::string>	split_conf;

	file.open(file_name);
	if (!file.is_open())
	{
		std::cout << "Wrong file name ! \n";
		exit(0);
	}
	str = read_a_file(file);
	file.close();
	split_conf = ft_split(str, "\n");

	size_t line_count = 0;
	while (line_count < split_conf.size())
	{
		std::vector<std::string> line = ft_split(split_conf[line_count], " \t;");
		if (line[0] == "server")
		{
			Server  *server = parse_server(split_conf, &line_count);
			_servers.push_back(server);
		}
		else
		{
			std::cout << "server is missing in config file";
			exit(1);
		}
		line_count++;
	}
}

