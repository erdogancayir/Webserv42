#pragma once
#include "webserv.hpp" 
#include "createSocket.hpp"
#include "handleCgi.hpp"
#include "client.hpp"


class reqHandler : handleCgi
{
	private:
		int						maxFd;
		int						loopControl;
		fd_set					readFds;
		fd_set					writeFds;
		fd_set					supReadFds;
		fd_set					supWriteFds;
		mysock					socketCreator;
		std::vector<int>		sockets;
		std::vector<Client *>	clients;
		std::string				errorPage;
		std::string				favico;
		std::string				forbiddenPage;

		void					findMaxFd();
		void					acceptProc(Webserv *websv);
		void					readProc(Webserv *websv);
		void					writeProc(void);
		int						findMySv(Client *cli, Webserv *websv);
		int						allowChecker(Client *cli);
		std::string				add_headers(std::string htmlstr, std::string type);
		int						deleteProc(Client *cli);
		std::string				read_a_file(std::fstream &file);
	public:
		reqHandler();
		void					initilizer(Webserv *websv);
		void					listenConn(Webserv *websv);
		void					close_connection(std::vector<Client *>::iterator cl, std::string page, bool isSend);
		std::string				add_headers(std::string htmlstr, std::string type, int code);
};