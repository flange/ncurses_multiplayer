#include <cstring>
#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "client.h"

Client::Client()
	: sock_(-1),
	  sockaddr_size_(sizeof(struct sockaddr_in))
{
	memset(&svr_addr_, 0, sockaddr_size_);
}

Client::~Client()
{
	close(sock_);
}

void Client::connect_to_svr(const char *ip, const uint16_t& port)
{
	open_tcp_socket();
	prepare_svr_addr(ip, port);

	int32_t ret = connect(sock_,
	                      (struct sockaddr *) &svr_addr_,
	                      sockaddr_size_);

	if (ret < 0)
		die(ClientError::conn_err);
}

void Client::disconnect()
{
	send(std::string("quit"));
}

void Client::send(const std::string& msg)
{
	// std::string.length() omits the '\0' char
	int32_t ret = write(sock_, msg.c_str(), msg.length() + 1);

	if (ret < 0)
		die(ClientError::write_err);
}

void Client::open_tcp_socket()
{
	sock_ = socket(AF_INET, SOCK_STREAM, 0);

	if (sock_ < 0)
		die(ClientError::sock_err);
}

void Client::prepare_svr_addr(const char *ip, const uint16_t& port)
{
	svr_addr_.sin_family = AF_INET;
	svr_addr_.sin_port   = htons(port);
	svr_addr_.sin_addr.s_addr = inet_addr(ip);
}

void Client::die(const ClientError& err)
{
	close(sock_);

	switch(err) {
	case ClientError::sock_err:
		std::cout << "Client.die(): socket() error" << std::endl;
		break;
	case ClientError::conn_err:
		std::cout << "Client.die(): connect() error" << std::endl;
		break;
	case ClientError::write_err:
		std::cout << "Client.die(): write() error" << std::endl;
		break;
  	}
}
