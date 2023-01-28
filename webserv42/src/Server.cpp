#include "../inc/Server.hpp"

Server::Server() { max_client_body_size = -1; } 

Server::~Server() { }

void	print_error(std::string error)
{
	std::cerr << "\nError : " << error << std::endl;
	exit(1);
}

int		Server::setup()
{
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sock;
	if (listen_fd == -1)
	{
		std::cerr << "Socket can't opened !" << std::endl;
		exit(1);
	}
	sock.sin_family = AF_INET;
	sock.sin_port = htons(port);

	inet_pton(listen_fd, ip_adress.c_str(), &sock.sin_addr);
	int opt = 1;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if (bind(listen_fd, (const struct sockaddr *)&sock, sizeof(sock)) == -1)
	{
		std::cerr << "Bind error !" << std::endl;
		exit(1);
	}
	listen(listen_fd, SOMAXCONN);
	return (listen_fd);
}

bool	Server::is_valid()
{
	int fd;

	if (ip_adress.size() == 0)
		print_error("Ip adress is not set !");
	if (port == 0)
		print_error("Port is not set !");
	if (root.size() == 0)
		print_error("Root directory is not set !");
	if (index.size() == 0)
		print_error("Index is not set !");
	
	std::map<unsigned int, std::string> error_pages = this->error_pages;
	for (std::map<unsigned int, std::string>::iterator error_page = error_pages.begin(); error_page != error_pages.end(); error_page++)
	{
		fd = ::open((root + error_page->second).c_str(), O_RDONLY);
		if (fd <= 0)
			print_error("error_page " + root + error_page->second + " is unavailable");
		close(fd);
	}
	return (true);
}