#include "../inc/client.hpp"

Client::Client(Server *sv, int fd)
{
	this->fd = fd;
	this->resp[0] = 0;
	this->sv = sv;
	this->status = 0;
	this->postLen = 0;
	this->code = 200;
}

void	Client::reqParser(char *req)
{
	size_t counter;
	size_t counter2;
	std::string tmp;
	std::string tmp1;
	int	isPost;
	status = 0;
	isPost = 0;

	if (req[0] == 'G' && req[1] == 'E' && req[2] == 'T' && req[3] == ' ')
	{
		this->method = "GET";
		counter = 4;
	}
	else if (req[0] == 'P' && req[1] == 'O' && req[2] == 'S' && req[3] == 'T' && req[4] == ' ')
	{
		this->method = "POST";
		isPost = 1;
		counter = 5;
	}
	else if (req[0] == 'D' && req[1] == 'E' && req[2] == 'L' && req[3] == 'E' && req[4] == 'T' && req[5] == 'E' && req[6] == ' ')
	{
		this->method = "DELETE";
		counter = 7;
	}
	else
	{
		this->status = -1;
		return ;
	}

	if (req[counter] != '/')
	{
		this->status = -1;
		return ;
	}

	counter2 = 0;
	while (req[counter] != ' ' && req[counter] != '?')
	{
		if (req[counter] == 0)
		{
			this->status = -1;
			return ;
		}
		this->dir += req[counter];
		counter2++;
		counter++;
	}

	if (req[counter] == '?')
	{
		counter++;
		while (req[counter] != ' ')
		{
			if (req[counter] == 0)
			{
				this->status = -1;
				return ;
			}
			this->queryString += req[counter];
			counter++;
		}
	}
	counter++;
	counter2 = 0;
	while (req[counter] != '\r' && req[counter + 1] != '\n')
	{
		if (req[counter] == 0)
		{
			this->status = -1;
			return ;
		}
		this->protocol += req[counter];
		counter2++;
		counter++;
	}
	counter += 2;
	while (!(req[counter] == '\r' && req[counter + 1] == '\n'))
	{
		if (req[counter] == 0)
		{
			this->status = -1;
			return ;
		}
		counter2 = 0;
		while (req[counter] != ':')
		{
			if (req[counter] == 0)
			{
				this->status = -1;
				return ;
			}
			tmp += req[counter];
			counter++;
			counter2++;
		}
		counter += 2;
		counter2 = 0;
		while (!(req[counter] == '\r' && req[counter + 1] == '\n'))
		{
			if (req[counter] == 0)
			{
				this->status = -1;
				return ;
			}
			tmp1 += req[counter];
			counter++;
			counter2++;
		}
		counter+= 2;
		if (tmp1.find("multi") != std::string::npos)
			this->multi = 1;
		else if (tmp == "Content-Length")
			this->postLen = atoi(tmp1.c_str());
		else if (tmp == "Host")
		{
			counter2 = tmp1.find(':');
			if (counter2 != std::string::npos)
			{
				this->serverName = tmp1.substr(0, counter2);
				if (this->serverName == "127.0.0.1")
					this->serverName = "localhost";
			}
		}
		this->headers.insert(std::make_pair(tmp, tmp1));
		tmp.clear();
		tmp1.clear();
	}
	counter += 2;
	if (isPost == 1 && this->multi != 1)
	{
		counter2 = 0;
		while (req[counter] != 0)
		{
			this->postVal += req[counter];
			counter++;
			counter2++;
		}
		this->postVal[counter2] = 0;
	}
	else if (req[counter] != 0)
	{
		this->status = -1;
		return ;
	}
	if (isPost == 1)
	{
		if (postVal.size() > this->sv->max_client_body_size)
			this->code = 303;
	}
	this->status = 1;
}
