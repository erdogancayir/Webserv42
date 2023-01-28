#include "../inc/createSocket.hpp"

mysock::mysock() { }

mysock::~mysock() { }


int	mysock::createSocket(int portNo)
{
	int optVal;
	struct sockaddr_in serveraddr;

	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->sockfd < 0)
	{
		std::cout << "Socket Cannot Created!" << std::endl;
		exit(-1);
	}
	fcntl(this->sockfd, F_SETFL, O_NONBLOCK);
	optVal = 1;	
	if (setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optVal, sizeof(int)) < 0)
	{
		std::cout << "SO_REUSEADDR Error!"<< std::endl;
		close(this->sockfd);
		exit(1);
	}
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)portNo);
	if (bind(this->sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
		return (-1);
	if (listen(this->sockfd, 128) < 0)
		return (-1);
	return (this->sockfd);
}