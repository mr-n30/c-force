#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <netdb.h>

#include "./brute.h"

void usage(int, char *);

int main(int argc, char **argv)
{
  usage(argc, argv[0]);
  hostname = argv[1];
  port = atoi(argv[2]);
  wordlist = argv[3];

  printf("Target: %s:%d\r\n", hostname, port);
  printf("Wordlist: %s\r\n", wordlist);
  printf("RESOURCE\t| STATUS\r\n");
  printf("==================================\r\n");
  brute(hostname, wordlist);

  return 0;
}

void usage(int argc, char *argv)
{
  if (argc != 4)
  {
    fprintf(stderr, "Usage: %s <host> <port> <wordlist>\r\n", argv);
    exit(2);
  }
}
