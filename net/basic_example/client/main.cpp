#include <cstdlib>
#include <iostream>
#include <string>

#include "client.h"

int main(int argc, char *argv[])
{
	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
		return EXIT_FAILURE;
	}

	char *ip = argv[1];
	uint16_t port = atoi(argv[2]);

	Client client;
	client.connect_to_svr(ip, port);

	std::string input;

	while (true) {
		std::getline(std::cin, input);

		if (input == "quit") {
			client.disconnect();
			break;
		} else {
			client.send(input);
		}
	}

	return EXIT_SUCCESS;
}
