#include "../inc/handleCgi.hpp"

void	free_2d_array(char **array)
{
	for (int i = 0; array[i]; i++)
		free(array[i]);
	free(array);
}

std::string	handleCgi::cgiEnv(Location *lit, std::string cgiPath, Client *cli)
{
	unsigned int counter;
	std::map<std::string, std::string>::iterator it;
	std::map<int, std::string> _env;
	char cwd[4096];

	getcwd(cwd, 4096);

	it = cli->headers.begin();
	counter = 0;
	while (it != cli->headers.end())
	{
		if ((*it).first == "Content-Type")
		{
			_env[counter] = "CONTENT_TYPE=" + (*it).second;
			counter++;
		}
		else if ((*it).first == "Content-Length")
		{
			_env[counter] = "CONTENT_LENGTH=" + (*it).second;
			counter++;
		}
		it++;
	}
	counter++;
	_env[counter] = "REQUEST_METHOD=" + cli->method;
	counter++;
	if (lit != NULL)
	{
			_env[counter] = "REQUEST_URI=" + lit->path  + "/" + lit->index;
			counter++;
	}
	else
	{
		_env[counter] = "REQUEST_URI=/" +  cli->sv->index;
		counter++;
		_env[counter] = "HTTP_HOST=" + cli->sv->root + "/" + cli->sv->index;;
	}
	counter++;
	_env[counter] = "SERVER_PROTOCOL=" + cli->protocol;
	counter++;
	_env[counter] = "REDIRECT_STATUS=200";
	counter++;
	_env[counter] = "SERVER_SOFTWARE=Webserv/1.0";
	counter++;
	_env[counter] = "SERVER_PORT=" + std::to_string(cli->sv->port);
	counter++;
	_env[counter] = "GATEWAY_INTERFACE=CGI/1.1";
	counter++;
	if (lit == NULL)
	{
		_env[counter] = "PATH_INFO=" + cli->sv->index;
			counter++;
		_env[counter] = "SCRIPT_FILENAME=" + cli->sv->root + "/" + cli->sv->index;
	}
	else
	{
		_env[counter] = "PATH_INFO=" + lit->path + "/" + lit->index; 
			counter++;
		_env[counter] = "SCRIPT_FILENAME=" + (std::string)cwd + "/" + cli->sv->root + lit->location_root + "/" + lit->index;
		counter++;
		_env[counter] = "SCRIPT_NAME=" + (std::string)cwd + "/" + cli->sv->root + lit->location_root + "/" + lit->index; 
	}
	counter++;
	if (cli->method == "GET")
		_env[counter] = "QUERY_STRING=" + cli->queryString;
	counter++;
	return (this->cgiRunner(_env, cgiPath, lit, cli));
}

char	**handleCgi::envToDoubleChar(std::map<int, std::string> &_env)
{
	char	**env = new char*[_env.size() + 1];
	int	j = 0;
	for (std::map<int, std::string>::const_iterator i = _env.begin(); i != _env.end(); i++) {
		std::string	element = i->second;
		env[j] = new char[element.size() + 1];
		env[j] = strcpy(env[j], (const char*)element.c_str());
		j++;
	}
	env[j] = NULL;
	return (env);
}

std::string	handleCgi::cgiRunner(std::map<int, std::string> _env, std::string cgiPath,  Location *lit, Client *cli)
{
	char	output[4096];
	int		readed;
	int	pipeFd[2];
	int	pipeo[2];
	std::string tmp;
	std::string tmp2;
	char *av1 = (char *)cgiPath.c_str();
	char *av2;
	char *av[3];
	char cwd[4096];

	getcwd(cwd, 4096);

	av[2] = 0;
	if (lit != NULL)
	{
		tmp = (std::string)cwd + "/" + cli->sv->root + lit->location_root + "/" + lit->index;
		av2 = (char *)tmp.c_str();
	}
	else
	{
		tmp = cli->sv->root + "/" + cli->sv->index;
		av2 = (char *)tmp.c_str();
	}
	av[0] = av1;
	av[1] = av2;
	char **env = this->envToDoubleChar(_env);

	pipe(pipeFd);
	pipe(pipeo);
	if (cli->method.find("POST") != std::string::npos)
		write(pipeFd[1], cli->postVal.c_str(), cli->postVal.length());

	close(pipeFd[1]);

	if (!fork())
	{

		close(pipeo[0]);
		dup2(pipeo[1], 1);
		close(pipeo[1]);

		if (cli->method.find("POST") != std::string::npos)
			dup2(pipeFd[0], 0);
		close(pipeFd[0]);

		execve(av[0], av, env);
		std::cout << "Execv Err!" << std::endl << std::flush;
		exit(-1);
	}
	wait(NULL);
	close(pipeFd[0]);
	close(pipeo[1]);

	readed = read(pipeo[0], output, 4096);
	if (readed == 0)
		std::cout << "Cgi Read Fail!" << std::endl << std::flush;
	close(pipeo[0]);
	output[readed] = 0;
	free_2d_array(env);
	return (std::string(output, readed));
}
