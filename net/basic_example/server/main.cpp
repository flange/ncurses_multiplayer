#include <cstdlib>
#include <cinttypes>
#include <iostream>

#include "server.h"

int main(int argc, char *argv[])
{
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <port>" << std::endl;
    return EXIT_FAILURE;
  }

  uint16_t port = atoi(argv[1]);

  Server svr;
  svr.serve_forever(port);

  return EXIT_SUCCESS;
}
