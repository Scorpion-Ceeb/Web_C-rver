
#include "Client.h"
#include "Extra_Methods.h"
#include "html_builder.h"
#include "Server.h"

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#define MAX_BUFFER_SIZE 1024
#define DELIMITATORS " \t\r\n"




void *client_handler(void *client_data)
{
    struct Client client = *(struct Client *) client_data;
    int client_socket = client.socket;
    char *client_root_path = client.root_path;
    int not_found = 0;

    char buffer[MAX_BUFFER_SIZE];

    recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);

    char **data_splitted = split_line_with_separators(buffer, DELIMITATORS);

    if(data_splitted[0] != NULL && strcmp(data_splitted[0], "GET") == 0 && data_splitted[1] != NULL)
    {
        char *path_server = path_to_server(data_splitted[1], client_root_path);

        int rp = to_other_url(client_socket, path_server, client_root_path);

        rp = rp || send_file_to_ulr(client_socket, path_server);

        if(!rp) not_found= 1;

        free(path_server);
    }
    else
    {
        not_found=1;
    }

    if( not_found)
    {
        char *not_found_response = "HTTP/1.1 404 NOT FOUND";
        send(client_socket, not_found_response, strlen(not_found_response), 0);
    }

    close(client_socket);

    free(data_splitted);
    free(client.root_path);
    
    return NULL;
}


