#include "../inc/reqHandler.hpp"

void	reqHandler::writeProc()
{
	int isFound;
	int	sendVal;

	isFound = 0;


	for(std::vector<Client *>::iterator cliIt = this->clients.begin(); this->loopControl && cliIt != this->clients.end(); cliIt++)
	{
		if (FD_ISSET((*cliIt)->fd, &supWriteFds))
		{
			if ((*cliIt)->dir == "/favicon.ico")
			{
				isFound = 1;
				sendVal = send((*cliIt)->fd, this->favico.c_str(), this->favico.length(), 0);
			}
			else if ((*cliIt)->dir == "/" || (*cliIt)->dir == "/" + (*cliIt)->sv->index)
			{
				isFound = 1;
				(*cliIt)->resp = (*cliIt)->sv->rawIndex;
				
				for(std::map<std::string, std::string>::iterator cgiIt = (*cliIt)->sv->cgis.begin(); cgiIt != (*cliIt)->sv->cgis.end(); cgiIt++)
				{
					if ((*cliIt)->sv->index.find((*cgiIt).first) != std::string::npos)
					{
						(*cliIt)->resp = this->cgiEnv(NULL, (*cgiIt).second, (*cliIt));
						break;
					}
				}

				(*cliIt)->resp = add_headers((*cliIt)->resp, "text/html", (*cliIt)->code);
				sendVal = send((*cliIt)->fd, (*cliIt)->resp.c_str(), (*cliIt)->resp.length(), 0);
			}
			else
			{
				// location check
				for(std::list<Location>::iterator locIt = (*cliIt)->sv->locations.begin(); locIt != (*cliIt)->sv->locations.end(); locIt++)
				{
					if ((*locIt).path + "/" + (*locIt).index   == (*cliIt)->dir)
					{
						isFound = 1;
						(*cliIt)->resp = (*locIt).rawIndex;
						for(std::map<std::string, std::string>::iterator cgiIt = (*cliIt)->sv->cgis.begin(); cgiIt != (*cliIt)->sv->cgis.end(); cgiIt++)
						{
							if ((*locIt).index.find((*cgiIt).first) != std::string::npos)
							{
								(*cliIt)->resp = this->cgiEnv(&(*locIt), (*cgiIt).second, (*cliIt));
								break;
							}
						}
						(*cliIt)->resp = add_headers((*cliIt)->resp, "text/html", (*cliIt)->code);
						sendVal = send((*cliIt)->fd, (*cliIt)->resp.c_str(), (*cliIt)->resp.length(), 0);
						break;
					}
				}
				// redirection check
				for(std::list<Location>::iterator locIt = (*cliIt)->sv->locations.begin(); locIt != (*cliIt)->sv->locations.end(); locIt++)
				{
					if ((*locIt).path == (*cliIt)->dir.substr(0, ((*locIt).path.size())))
					{
						if ((*locIt).HTTP_redirection_type == 0)
							continue; ;
						std::string _str = "";
						std::string line;
						std::string _path = (*cliIt)->sv->root + "/" + (*locIt).location_root + "/" + (*locIt).HTTP_redirection;
						while (_path.find("//") != std::string::npos)
							_path.replace(_path.find("//"), 2, "/");
						std::fstream tmp_fd(_path);
						if (!tmp_fd.is_open())
						{
							std::cerr << "Location Error " << std::endl;
							exit(1);
						}
						if (tmp_fd.eof()) {
    						std::cout << "Return file is empty!?" << std::endl;
							exit(1);
						}
    					_str = read_a_file(tmp_fd);
						isFound = 1;
						tmp_fd.close();
						_str = add_headers(_str, "text/html", (*locIt).HTTP_redirection_type);
						sendVal = send((*cliIt)->fd, _str.c_str(), _str.length(), 0);
						break ;
					}
				}
			}

			if (isFound == 1)
			{
				if (sendVal > 0)
					std::cout << "Send SuccesFully!" << std::endl << std::flush;
				close_connection(cliIt, errorPage, false);
			}
			else
				close_connection(cliIt, errorPage, true);
			this->loopControl = 0;
			break;
		}
	}
}