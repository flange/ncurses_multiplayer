#include <cstdlib>
#include <cstring>
#include <iostream>

#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "server.h"

Server::Server()
	: svr_sock_(-1),
	  cli_sock_(-1),
	  sockaddr_size_(sizeof(struct sockaddr_in))
{
	memset(buf, 0, buf_size_);
	memset(&svr_addr_, 0, sockaddr_size_);
	memset(&cli_addr_, 0, sockaddr_size_);
}

Server::~Server()
{
	close(svr_sock_);
	close(cli_sock_);
}

void Server::serve_forever(uint16_t port)
{
	open_tcp_sock();
	prepare_svr_addr(port);
	bind_sock();
	start_listening();

	int32_t ret;

	while (true) {
		cli_sock_ = accept(svr_sock_,
		                  (struct sockaddr *) &cli_addr_,
		                  (socklen_t *) &sockaddr_size_);
		if (cli_sock_ < 0)
			die(ServerError::accept_err);

		std::cout << "Connection established." << std::endl;

		while (true) {
			memset(buf, 0, buf_size_);

			ret = read(cli_sock_, buf, buf_size_ - 1);

			if (ret < 0)
				die(ServerError::read_err);

			if (!strcmp(buf, "quit")) {
				std::cout << "Connection terminated.\n" << std::endl;
				break;
			} else {
				std::cout << buf << std::endl;
			}
		}
	}
}

void Server::open_tcp_sock()
{
	svr_sock_ = socket(AF_INET, SOCK_STREAM, 0);

	if (svr_sock_ < 0)
		die(ServerError::sock_err);
}

void Server::prepare_svr_addr(const uint16_t& port)
{
	svr_addr_.sin_family = AF_INET;
	svr_addr_.sin_port = htons(port);
	svr_addr_.sin_addr.s_addr = INADDR_ANY;
}

void Server::bind_sock()
{
	int32_t ret = bind(svr_sock_,
	                   (struct sockaddr *) &svr_addr_,
	                   sizeof(struct sockaddr_in));

	if (ret < 0)
		die(ServerError::bind_err);
}

void Server::start_listening()
{
	int32_t ret = listen(svr_sock_, 1);

	if (ret < 0)
		die(ServerError::listen_err);
}

void Server::die(const ServerError& err)
{
	close(svr_sock_);
	close(cli_sock_);

	switch(err) {
	case ServerError::sock_err:
		std::cout << "Server.die(): socket() error" << std::endl;
		break;
	case ServerError::bind_err:
		std::cout << "Server.die(): bind() error" << std::endl;
		break;
	case ServerError::listen_err:
		std::cout << "Server.die(): listen() error" << std::endl;
		break;
	case ServerError::accept_err:
		std::cout << "Server.die(): accept() error" << std::endl;
		break;
	case ServerError::read_err:
		std::cout << "Server.die(): read() error" << std::endl;
		break;
	}

	std::exit(EXIT_FAILURE);
}
