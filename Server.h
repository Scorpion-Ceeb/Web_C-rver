#ifndef Server_h
#define Server_h

#include <sys/socket.h>
#include <netinet/in.h>


struct Server
{
    int domain;
    int service;
    int protocol;
    int port;
    int backlog;

    int socket;

    struct sockaddr_in address;
    
};

int server_constructor(char *server_ip, int port, int backlog);

int to_other_url(int client_socket, char *path, char *root_path);

int send_file_to_ulr(int client_socket, char *path);

#endif /* Server_h */