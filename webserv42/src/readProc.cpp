#include "../inc/reqHandler.hpp"
#include <fstream>
#include <cstdio>
#include <iostream>

int	reqHandler::deleteProc(Client *cli)
{
	std::string tmp;
	std::string tmp2;
	size_t len = cli->dir.length() - 1;
	size_t len2 = len;
	int	success;
	success = -1;


	while (len != 0 && cli->dir.c_str()[len] != '/')
		len--;
	len2 = len;
	while (len < cli->dir.length())
	{
		tmp += cli->dir.c_str()[len];
		len++;
	}


	len = 0;
	while (len < len2)
	{
		tmp2 += cli->dir.c_str()[len];
		len++;
	}

	for(std::list<Location>::iterator locationIt = cli->sv->locations.begin(); locationIt != cli->sv->locations.end(); locationIt++)
	{
		if (tmp2 == (*locationIt).location_root + (*locationIt).path)
		{
			for(std::list<std::string>::iterator allowIt = (*locationIt).allow.begin(); allowIt != (*locationIt).allow.end(); allowIt++)
			{
				if ((*allowIt) == cli->method)
				{
					char cwd[4096];
					getcwd(cwd, 4096);
					success = remove(((std::string)cwd + "/" + cli->sv->root + ((*locationIt).path + tmp)).c_str());
					return (success);
				}
			}
		}
	}
	return (success);
}

void	reqHandler::close_connection(std::vector<Client*>::iterator cl, std::string page, bool isSend)
{
	if (isSend)
		send((*cl)->fd, page.c_str(), page.length(), 0);
	FD_CLR((*cl)->fd, &this->readFds);
	FD_CLR((*cl)->fd, &this->writeFds);
	close((*cl)->fd);
	delete((*cl));
	this->clients.erase(cl);
	this->findMaxFd();
}

void	reqHandler::readProc(void)
{
	int	readed;
	char buffer[4096] = {0};
	size_t	sup_len;

	readed = -1;
	for(std::vector<Client *>::iterator cliIt = this->clients.begin(); this->loopControl && cliIt != this->clients.end(); cliIt++)
	{
		if (FD_ISSET((*cliIt)->fd, &supReadFds))
		{
			readed = recv((*cliIt)->fd, buffer, 4095, 0);
			if (readed > 0)
			{
				if ((*cliIt)->status == 0)
				{
					(*cliIt)->reqParser(buffer);
					if ((*cliIt)->status == 1)
					{
						if (this->allowChecker((*cliIt)) == -1)
							close_connection(cliIt, forbiddenPage, true);
						else
						{
							if ((*cliIt)->method == "DELETE")
							{
								if (deleteProc((*cliIt)) == -1)
									send((*cliIt)->fd, "False!" , 6, 0);
								else
									send((*cliIt)->fd, "True!" , 5, 0);
								close_connection(cliIt, errorPage, false);
							}
							else
							{
								if ((*cliIt)->multi == 1)
								{
									if ((*cliIt)->postLen == (*cliIt)->postVal.length())
										FD_SET((*cliIt)->fd, &this->writeFds);
								}
								else
								{
									FD_CLR((*cliIt)->fd, &this->readFds);
									FD_SET((*cliIt)->fd, &this->writeFds);
								}
							}
						}
					}
					else
					{
						close_connection(cliIt, errorPage, false);
						std::cout << "Wrong Data!" << std::endl << std::flush;
					}
				}
				else if ((*cliIt)->status == 1 && (*cliIt)->multi == 1)
				{
					sup_len = (*cliIt)->postVal.length();
					if (!(sup_len + readed >= std::string::npos))
					{
						(*cliIt)->postVal += std::string(buffer, readed);
						if ((*cliIt)->postLen == (*cliIt)->postVal.length())
							FD_SET((*cliIt)->fd, &this->writeFds);
					}
					else
					{
						std::cout << "So Big!" << std::endl << std::flush;
						close_connection(cliIt, errorPage, false);
					}
				}
			}
			else
			{
				close_connection(cliIt, errorPage, false);
				std::cout << "Connection Closed!" << std::endl << std::flush;
			}
			this->loopControl = 0;
			break;
		}
	}
}
