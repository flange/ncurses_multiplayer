#ifndef SERVER_H
#define SERVER_H

#include <cinttypes>
#include <cstring>

#include <netinet/in.h>
#include <sys/socket.h>

enum class ServerError {
	sock_err,
	bind_err,
	listen_err,
	accept_err,
	read_err
};

class Server {
public:
	Server();
	~Server();
	void serve_forever(uint16_t port);

private:
	void open_tcp_sock();
	void prepare_svr_addr(const uint16_t& port);
	void bind_sock();
	void start_listening();
	void die(const ServerError& err);

private:
	static const uint8_t buf_size_ = 32;

	char buf[buf_size_];

	int32_t svr_sock_;
	int32_t cli_sock_;

	size_t sockaddr_size_;
	struct sockaddr_in svr_addr_;
	struct sockaddr_in cli_addr_;
};

#endif // SERVER_H
