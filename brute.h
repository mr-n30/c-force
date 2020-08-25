/* GLOBALS */
char *hostname;
char *wordlist;
unsigned port;

struct in_addr *resolve_command_line_argv(char *ip_address)
{
  struct hostent *host_info;
  struct in_addr *address;
  host_info = gethostbyname(ip_address);
  address = (struct in_addr *) (host_info->h_addr);
  // FOR DEBUGING HOSTNAME BEING RESOLVED
  //printf("%s has address %s\n", ip_address, inet_ntoa(*address));

  return address;
}

void send_data_to_server(int sockfd, char *string_to_send, char *buffer)
{
  int string_length = strlen(string_to_send);
  send(sockfd, string_to_send, string_length, 0);

  char response_code[256];
  read(sockfd, response_code, 12);
  if ((strncmp(response_code, "HTTP/1.1 200", 12)) == 0)
    printf("%s\t=>\t200\n", (strtok(buffer, "\n")));

  return;
}

void create_socket(char *hostname, char *resource, char *buffer)
{
  int sockfd;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  int opt=1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  /* Resolve argv[1] to IP */
  struct in_addr * ip_address = resolve_command_line_argv(hostname);

  /* Create client struct */
  struct sockaddr_in client_addr;
  client_addr.sin_family = AF_INET;
  client_addr.sin_port   = htons(port);
  client_addr.sin_addr   = *ip_address;
  memset(client_addr.sin_zero, '\0', 8);

  connect(sockfd, (struct sockaddr *) &client_addr, sizeof(struct sockaddr));

  send_data_to_server(sockfd, resource, buffer);

  close(sockfd);

  return;
}

int brute(char *hostname, char *wordlist)
{
  char *resource = (char *) malloc((sizeof(char) * 256) * 2);
  char buffer[(sizeof(char)*256)];
  FILE *file_pointer = fopen(wordlist, "r");
  // FOR DEBUGING OPENING FILE
  //printf("Wordlist file: %s\n", wordlist);

  while(fgets(buffer, sizeof(buffer), file_pointer))
  {
    strcpy(resource, "GET /");
    strcat(resource, buffer);
    strtok(resource, "\n");
    strcat(resource, " HTTP/1.1\r\n");
    strcat(resource, "Host: ");
    strcat(resource, hostname);
    strcat(resource, "\r\nConnection: close\r\n\r\n");
    create_socket(hostname, resource, buffer);
  }

  free(resource);
  fclose(file_pointer);

  return 0;
}
