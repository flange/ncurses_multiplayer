#ifndef CLIENT_H
#define CLIENT_H

#include <cinttypes>
#include <cstring>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

enum class ClientError {
	sock_err,
	conn_err,
	write_err
};

class Client {
public:
	Client();
	~Client();
	void connect_to_svr(const char *ip, const uint16_t& port);
	void disconnect();
	void send(const std::string& msg);

private:
	void open_tcp_socket();
	void prepare_svr_addr(const char *ip, const uint16_t& port);
	void die(const ClientError& err);

private:
	int32_t sock_;

	size_t sockaddr_size_;
	struct sockaddr_in svr_addr_;
};

#endif // CLIENT_H
