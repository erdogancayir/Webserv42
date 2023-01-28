#include "../inc/reqHandler.hpp"

void	reqHandler::acceptProc(Webserv *websv)
{
	int forAccept;
	Client *cli;

	for(std::vector<int>::iterator it = this->sockets.begin(); this->loopControl && it != this->sockets.end(); it++)
	{
		if(FD_ISSET((*it), &supReadFds))
		{
			for(std::list<Server*>::iterator serverIt = websv->_servers.begin(); serverIt != websv->_servers.end(); serverIt++)
			{
				if ((*it) == (*serverIt)->listen_fd)
				{
					forAccept = accept((*it), NULL, NULL);
					if (forAccept != -1)
					{
						cli = new Client((*serverIt), forAccept);
						this->clients.push_back(cli);
						FD_SET(forAccept, &readFds);
						findMaxFd();
					}
				}
			}
			this->loopControl = 0;
			break;
		}
	}
}