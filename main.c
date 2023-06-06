#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <pwd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

#include "Server.h"
#include "Client.h"

#define MAX_SIZE_BUFFER 1024
#define BBlue "\033[1;34m"
#define RESET "\033[0m"

void launch(int port, char *root_path) {

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

int main(int argn, char *arg_values[]) 
{

    int port;
    char *root_path;

    uid_t uid;

    uid = getuid();
    struct passwd *pwd = getpwuid(uid);


    if (arg_values[1] == NULL) port = 5000;
    else {
        char *p;
        int q = (int) strtol(arg_values[1], &p, 10);

        if (strlen(p) == 0) port = q;
        else {
            fprintf(stderr, "%s: the port is not valid\n", "ERROR");

            return 0;
        }
    }

    if (arg_values[1] == NULL || arg_values[2] == NULL) {
        root_path = pwd->pw_dir;
    } else {
        DIR *directory;
        directory = opendir(arg_values[2]);

        if (directory) root_path = arg_values[2];
        else {
            fprintf(stderr, "%s: the path does not exist\n", "ERROR");

            return 0;
        }
        closedir(directory);
    }

    char modified_path_to_print[MAX_SIZE_BUFFER];
    char modified_url_to_print[MAX_SIZE_BUFFER];

    sprintf(modified_path_to_print, "%sPath%s: %s", BBlue, RESET, root_path);
    sprintf(modified_url_to_print, "%sURL%s:  http://localhost:%d", BBlue, RESET, port);


    puts(modified_path_to_print);
    puts(modified_url_to_print);


    launch(port, root_path);

}