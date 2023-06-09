
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <pthread.h>

#include "html_builder.h"
#include "Extra_Methods.h"
#include "Server.h"
#include "Client.h"
#include "Url_Methods.h"

#define MAX_SIZE_BUFFER 1024
#define DELIMITATORS " \t\r\n\a"


int server_constructor(int domain, int service, int protocol, char *server_ip, int port, int backlog) 
{
    struct sockaddr_in server_address;
    int server_socket;

    server_socket = socket(domain, service, protocol);

    if (server_socket < 0) {
        fprintf(stderr, "%s: socket creation failed\n", "ERROR");
        exit(1);
    }

    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &(int) {1}, sizeof(int));

    server_address.sin_family = domain;
    server_address.sin_port = htons(port);
    inet_aton(server_ip, &server_address.sin_addr);

    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) 
    {
        fprintf(stderr, "%s: binding failed\n", "ERROR");
        exit(1);
    }

    if (listen(server_socket, backlog) < 0) 
    {
        fprintf(stderr, "%s: listen failed\n", "ERROR");
        exit(1);
    }

    return server_socket;
}

int to_other_url(int client_socket, char *path, char *root_path) 
{
    DIR *directory;
    directory = opendir(path);

    if (directory) {
        char *response = render_html(directory, path, root_path);

        send(client_socket, response, strlen(response), 0);
        free(response);
        closedir(directory);

        return 1;
    }
    closedir(directory);

    return 0;
}

int send_file_to_ulr(int client_socket, char *path) 
{

    off_t offset = 0;
    struct stat stat_buffer;
    
    int stream = open(path, O_RDONLY);
    if (stream < 0) {
        return 0;
    }

    
    fstat(stream, &stat_buffer);

    char head[MAX_SIZE_BUFFER];
    snprintf(head, sizeof(head), "HTTP/1.1 200 OK\r\n"
                                     "Content-Type: application/octet-stream\r\n"
                                     "Content-Disposition: attachment; filename=\"%s\"\r\n"
                                     "Content-Length: %ld\r\n"
                                     "\r\n", path, stat_buffer.st_size);

    send(client_socket, head, strlen(head), 0);
    sendfile(client_socket, stream, &offset, stat_buffer.st_size);

    close(stream);

    return 1;
}


void launch(int port, char *root_path) 
{

    int socket_server = server_constructor(AF_INET, SOCK_STREAM, 0, "localhost", port, 1);

    while (1) {
        struct sockaddr_in client_address;
        int length_client_address = sizeof(client_address);
        int sock_client = accept(socket_server, (struct sockaddr *) &client_address, (socklen_t *) &length_client_address);

        struct Client client;
        client.socket = sock_client;
        client.root_path = (char *) malloc(MAX_SIZE_BUFFER);
        strcpy(client.root_path, root_path);

        pthread_t pthread;
        pthread_create(&pthread, NULL, client_handler, &client);
    }
}
