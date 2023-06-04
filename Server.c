

#include "Server.h"
#include "Build_Html.h"


#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>


#define MAX_BUFFER_SIZE 1024

int server_constructor(char *server_ip, int port, int backlog)
{
    struct Server server;

    server.port= port;
    server.backlog= backlog;


    int socket1 = socket(AF_INET, SOCK_STREAM, 0);

    if( server.socket < 0){
        perror("Failed to connect socket...\n");
        exit(1);
    }

    setsockopt(socket1, SOL_SOCKET, SO_REUSEADDR, &(int) {1}, sizeof(int));

    struct sockaddr_in address = {0};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    inet_aton(server_ip, &address.sin_addr);


    if((bind(socket1, (struct sockaddr *)&address, sizeof(address))) < 0)
    {
        perror("Failed to bind socket ... \n");
        exit(1);
    }

    if((listen(socket1, server.backlog)) < 0){
        perror("Failed to start listen... \n");
        exit(1);
    }

    return socket1;
}


int to_other_url(int socket_client, char *path, char *root_path)
{
    DIR *dir;
    dir = opendir(path);

    if(dir){
        char *html = render_html(dir, path, root_path);

        send(socket_client, &html, strlen(html), 0);

        closedir(dir);
        return 1;
    }

    closedir(dir);

    return 0;
}

int send_file_to_ulr(int socket_client, char *path)
{
    int stream = open(path, O_RDONLY);

    if (stream == -1) return -1;

    off_t off_set = 0;
    struct stat stat_buffer;

    fstat(stream, &stat_buffer);

    char buffer[MAX_BUFFER_SIZE];

    /*Check this*/
    snprintf(buffer, sizeof(buffer), "HTTP/1.1 200 OK\r\n"
                                     "Content-Type: application/octet-stream\r\n"
                                     "Content-Disposition: attachment; filename=\"%s\"\r\n"
                                     "Content-Length: %ld\r\n"
                                     "\r\n", path, stat_buffer.st_size);

    send(socket_client, buffer, sizeof(buffer), 0);
    sendfile(socket_client, stream, &off_set, stat_buffer.st_size);

    close(stream);

    return 1;
}
