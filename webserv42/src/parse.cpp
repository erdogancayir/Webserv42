#include "../inc/webserv.hpp"

static bool	ft_in_charset(char const c, const std::string &charset)
{
	int	i_charset;

	i_charset = 0;
	while (charset[i_charset])
	{
		if (c == charset[i_charset++])
			return true;
	}
	return false;
}

void	error_handle(std::string msg)
{
	std::cerr << msg << std::endl;
	exit(1);
}

std::vector<std::string>	ft_split(const std::string &str, const std::string &charset)
{
	std::vector<std::string>	res;
	std::string					tmp;
	size_t						i;

	i = 0;
	while (i < str.size())
	{
		while (i < str.size() && ft_in_charset(str[i], charset))
			i++;
		if (i < str.size())
		{
			tmp = "";
			while (i < str.size() && !ft_in_charset(str[i], charset))
				tmp += str[i++];
			res.push_back(tmp);
		}
	}
	return res;
}

Location    parse_location(const std::vector<std::string> &config, size_t *line_count, Server *server)
{
	Location	location;
	std::vector<std::string> line = ft_split(config[*line_count], "; \t");

	if (line.size() != 3)
		error_handle("expecting a directory and '{' after 'location'");
	std::string tmp = line[1];
	if (tmp.size() > 1 && tmp[tmp.size() - 1] == '/')
		tmp.resize(tmp.size() - 1);
	location.path = tmp;
	if (line[2] != "{")
		error_handle("expecting '{' after 'location' directory");
	(*line_count)++;
	std::vector<std::string>::const_iterator it = config.begin() + *line_count;
	while (it != config.end())
	{
		std::vector<std::string> line = ft_split(*it, " \t;");
		if (!line.size() || !line[0].size() || line[0][0] == '#')
		{
			it++;
			(*line_count)++;
			continue;
		}
		if (line[0] == "}")
			break;
		else if (line[0] == "allow")
		{
			if (line.size() < 2)
				error_handle("expecting at least 1 argument after 'allow'");
			for (std::vector<std::string>::const_iterator allow_m = line.begin() + 1; allow_m != line.end(); allow_m++)
				location.allow.push_back(*allow_m);
		}
		else if (line[0] == "return")
		{
			if (line.size() != 3)
				error_handle("expecting 2 arguments after 'return'");
			if (location.HTTP_redirection_type != 0)
				error_handle("only one redirection can be set for a given location");
			int type = atoi(line[1].c_str());
			if (!(type == 300 || type == 404 || type == 403))
				error_handle("redirection code is invalid");
			if (line[1].size())
			{
				location.HTTP_redirection_type = type;
				location.HTTP_redirection = line[2];
			}
		}
		else if (line[0] == "root")
		{
			if (line.size() != 2)
				error_handle("expecting 1 argument after 'root'");
			std::string tmp = line[1];
			if (tmp.size() > 1 && tmp[tmp.size() - 1] == '/')
				tmp.resize(tmp.size() - 1);
			location.location_root = tmp;
		}
		else if (line[0] == "index")
		{
			if (line.size() != 2)
				error_handle("expecting 1 argument after 'index'");
			location.index = line[1];
		}
		else
			std::cout << "\t\tDEBUG : ***OTHER LOCATION: " << line[0];
		it++;
		(*line_count)++;
	}
	std::vector<std::string> end = ft_split(config[*line_count], " \t");
	if (end[0] != "}")
		error_handle(" location: missing closing bracket");
	location.server = server;
	return location;
}

Server	*parse_server(const std::vector<std::string> config, size_t *line_count)
{
	std::vector<std::string> line = ft_split(config[0], " \t;");
	if (line[1] != "{")
		return (NULL);
	Server  *server = new Server();
	std::vector<std::string>::const_iterator it = config.begin() + *line_count;
	if (it == config.end())
		return (NULL);
	it++;
	(*line_count)++;
	while (it != config.end())
	{
		std::vector<std::string> line = ft_split(*it, " \t;");
		if (!line.size() || !line[0].size() || line[0][0] == '#')
		{
			it++;
			(*line_count)++;
			continue;
		}
		if (line[0] == "}")
			break;
		else if (line[0] == "location")
		{
			if (line.size() < 2)
				error_handle("expecting 2 arguments after 'location'" );
			size_t  old_line_count = *line_count;
			server->locations.push_back(parse_location(config, line_count, server));
			it += *line_count - old_line_count;
		}
		else if (line[0] == "server_name")
		{
			if (line.size() < 2)
				error_handle("expecting at least 1 argument after 'server_name'");
			for (std::vector<std::string>::const_iterator s_name = line.begin() + 1; s_name != line.end(); s_name++)
				server->server_names.push_back(*s_name);
		}
		else if (line[0] == "listen")
		{
			if (line.size() != 2)
				error_handle("expecting 1 argument after 'listen'");
			std::vector<std::string> listen = ft_split(line[1], ":");
			std::string adress;
			std::string port;
			adress = listen[0];
			if (listen.size() == 1)
			{
				adress = "0.0.0.0";
				port = listen[0];
			}
			else
				port = listen[1];
			if (adress == "localhost")
				adress = "127.0.0.1";
			server->ip_adress = adress;
			server->port = atoi(port.c_str());
		}
		else if (line[0] == "root")
		{
			if (line.size() != 2)
				error_handle("expecting 1 argument after 'root'");
			std::string tmp = line[1];
			if (tmp.size() > 1 && tmp[tmp.size() - 1] == '/')
				tmp.resize(tmp.size() - 1);
			server->root = tmp;
		}
		else if (line[0] == "max_client_body_size")
		{
			if (line.size() != 2)
				error_handle("expecting 1 argument after 'max_client_body_size'");
			server->max_client_body_size = atoll(line[1].c_str());
		}
		else if (line[0] == "index")
		{
			if (line.size() != 2)
				error_handle("expecting 1 argument after 'index'");
			server->index = line[1];
		}
		else if (line[0] == "cgi")
		{
			if (line.size() < 3)
				error_handle("expecting 2 argument after 'cgi'");
			server->cgis.insert(std::pair<std::string, std::string>(line[1], line[2]));
		}
		else if (line[0] == "error_page")
		{
			if (line.size() != 3)
				error_handle("expecting 3 arguments in 'error_page'");
			server->error_pages.insert(std::pair<unsigned int, std::string>(atoi(line[1].c_str()), line[2]));
		}
		it++;
		(*line_count)++;
	}
	std::vector<std::string> end = ft_split(*it, " /t");
	while (!end.size() || end[0] != "}")
	{
		(*line_count)++;
		end = ft_split(config[*line_count], " \t");
	}
	if (end.size() && end[0] != "}")
		error_handle("missing closing bracket");
	server->is_valid();
	return (server);
}
