#pragma once

#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <ostream>
#include <cstring>


class mysock
{
	protected:
		int	sockfd;
	public:
		int	createSocket(int portNo);
		int	getSockfd() { return(this->sockfd); };
		mysock();
		~mysock();
};

