#include "../inc/createSocket.hpp"
#include "../inc/reqHandler.hpp"
#include "../inc/webserv.hpp"
#include <signal.h>

Webserv *_end;

void	end(int signal, siginfo_t *siginfo, void *unused)
{
	(void)unused;
	(void)signal;
	(void)siginfo;
	for (std::list<Server *>::iterator server = _end->_servers.begin(); server != _end->_servers.end(); server++)
	{
		std::cout << "\n\tᴅᴇʟᴇᴛɪɴɢ ꜱᴇʀᴠᴇʀ" << std::endl;
		delete (*server);
		*server = 0;
	}
	_end->_servers.clear();
	std::cout << "\t 𝕔𝕝𝕠𝕤𝕖𝕕 " << std::endl;
	std::cout << "\t𝘣𝘺𝘦𝘦" << std::endl;
	system("leaks webserv");
	exit(0);
}

int	main(int argc, char **argv)
{
	struct sigaction	act;

	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = end;
	reqHandler *rH = new(reqHandler);

	if (argc != 2)
		error_handle("Error: wrong arguments !");

	Webserv webserv(argv[1]);

	rH->initilizer(&webserv);
	_end = &webserv;
	std::cout << "\033[32mLISTENING\n\033[0m";
	sigaction(SIGINT, &act, NULL);
	rH->listenConn(&webserv);

	delete rH;
}
