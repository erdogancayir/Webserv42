#include "../inc/reqHandler.hpp"

reqHandler::reqHandler()
{
	this->maxFd = 0;
	this->loopControl = 1;
}

std::string	reqHandler::add_headers(std::string htmlstr, std::string type, int code)
{
	std::string headers;
	headers += "HTTP/1.1 " + std::to_string(code) + " OK\n";
	headers += "Date: " + get_time_stamp() + "\n";
	headers += "Server: webserv/1.00\n";
	headers += "Content-Type: " + type + "\n";
	headers += "Content-Length: " + std::to_string(htmlstr.size()) + "\n";
	headers += "Connection: close\n";
	std::string res;
	res = headers + "\n" + htmlstr;
	return (res);
}


std::string	reqHandler::read_a_file(std::fstream &file)
{
	std::string res;
	std::string	tmp;

	while (getline(file, tmp))
		res += tmp + '\n';
	return (res);
}

void	open_read(std::string file_name, std::string &page)
{
	std::fstream fd(file_name);
	std::string tmp;

	if (!fd.is_open())
		error_handle("File directory error !");

	while (getline(fd, tmp))
		page += tmp + '\n';
	fd.close();
}

void	reqHandler::initilizer(Webserv *websv)
{
	int state;
	std::string tmp_string;
	std::fstream tmpfd;
	std::fstream tmpfd2;

	FD_ZERO(&this->writeFds);
	FD_ZERO(&this->readFds);
	FD_ZERO(&this->supReadFds);
	FD_ZERO(&this->supWriteFds);

	open_read("index/errpage404", errorPage);
	this->errorPage = add_headers(this->errorPage, "text/html", 404);

	open_read("index/errpage403", forbiddenPage);
	this->forbiddenPage = add_headers(this->forbiddenPage, "text/html", 403);

	open_read("index/canodis.svg", favico);
	this->favico = add_headers(this->favico, "image/svg+xml", 200);

	for (std::list<Server *>::iterator it = websv->_servers.begin(); it != websv->_servers.end(); it++)
	{
		state = 0;
		std::list<Server *>::iterator itch = websv->_servers.begin();
		for (; itch != it; itch++)
		{
			if ((*itch)->port == (*it)->port)
			{
				state = -1;
				break;
			}
		}
		if (state == 0)
		{
			state = this->socketCreator.createSocket( (int)(*it)->port );
			(*it)->listen_fd = state;
			sockets.push_back(state);
			FD_SET(state, &this->readFds);
			this->maxFd = state + 1;
		}
		else
			(*it)->listen_fd = (*itch)->listen_fd;
		tmp_string = (*it)->root + "/" + (*it)->index;
		while (tmp_string.find("//") != std::string::npos)
				tmp_string.replace(tmp_string.find("//"), 2, "/");
		tmpfd.open(tmp_string);
		if (!tmpfd.is_open())
			error_handle("\n ERROR wrong path server root or index");
		(*it)->rawIndex = this->read_a_file(tmpfd);
		tmpfd.close();
		for (std::list<Location>::iterator locit = (*it)->locations.begin(); locit != (*it)->locations.end(); locit++)
		{
			tmp_string = (*it)->root + "/" + (*locit).location_root + "/" + (*locit).index;
			while (tmp_string.find("//") != std::string::npos)
				tmp_string.replace(tmp_string.find("//"), 2, "/");
			tmpfd.open(tmp_string);
			if (!tmpfd.is_open())
			{
				if (tmp_string[tmp_string.size() - 1] == '/' || (*it)->index[0] == '/')
					tmp_string = tmp_string + (*it)->index;
				else
					tmp_string = tmp_string + "/" + (*it)->index;
				tmpfd.open(tmp_string);
			}
			if (!tmpfd.is_open())
				error_handle("Location Error ");
			(*locit).rawIndex = read_a_file(tmpfd);
			tmpfd.close();
		}
	}
}

void	reqHandler::findMaxFd()
{
	this->maxFd = -1;

	for(std::vector<int>::iterator it = this->sockets.begin(); it != this->sockets.end(); it++)
	{
		if ((*it) >= this->maxFd)
		{
			this->maxFd = (*it);
			this->maxFd += 1;
		}
	}

	for(std::vector<Client *>::iterator cliIt = this->clients.begin(); this->loopControl && cliIt != this->clients.end(); cliIt++)
	{
		if ((*cliIt)->fd >= this->maxFd)
		{
			this->maxFd = (*cliIt)->fd;
			this->maxFd += 1;
		}
	}
}

int	reqHandler::allowChecker(Client *cli)
{
	if (cli->method == "DELETE")
		return (1);
	if (cli->dir == cli->sv->index || cli->dir == "/" || cli->dir == "/favicon.ico")
		return (1);
	else
	{
		for(std::list<Location>::iterator locationIt = cli->sv->locations.begin(); locationIt != cli->sv->locations.end(); locationIt++)
		{
			if (cli->dir == (*locationIt).location_root + "/" + (*locationIt).index)
			{
				for(std::list<std::string>::iterator allowIt = (*locationIt).allow.begin(); allowIt != (*locationIt).allow.end(); allowIt++)
				{
					if ((*allowIt) == cli->method)
						return (1);
				}
				return (-1);
			}
		}
	}
	return (1);
}

void	reqHandler::listenConn(Webserv *websv)
{
	int	state;

	while (1)
	{
		state = 0;
		while (state == 0)
		{
			this->supWriteFds = this->writeFds;
			this->supReadFds = this->readFds;
			state = select(this->maxFd, &this->supReadFds, &this->supWriteFds, NULL, 0);
		}
		this->loopControl = 1;
		this->readProc();
		this->writeProc();
		this->acceptProc(websv);
	}
}