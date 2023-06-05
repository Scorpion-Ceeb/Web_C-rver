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
#define GREEN "\033[0;32m"
#define RESET "\033[0m"

void launch(int port, char *root_path) {

    int socket_server = server_constructor("localhost", port, 1);

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

int main(int argn, char *argv[]) {
    signal(SIGPIPE, SIG_IGN);

    int port;
    char *root_path;

    uid_t uid;

    uid = getuid();
    struct passwd *pwd = getpwuid(uid);


    if (argv[1] == NULL) port = 5000;
    else {
        char *p;
        int q = (int) strtol(argv[1], &p, 10);

        if (strlen(p) == 0) port = q;
        else {
            fprintf(stderr, "%s: the port is not valid\n", "ERROR");

            return 0;
        }
    }

    if (argv[1] == NULL || argv[2] == NULL) {
        root_path = pwd->pw_dir;
    } else {
        DIR *d;
        d = opendir(argv[2]);

        if (d) root_path = argv[2];
        else {
            fprintf(stderr, "%s: the path does not exist\n", "ERROR");

            return 0;
        }
        closedir(d);
    }

    char print_path[MAX_SIZE_BUFFER];
    char print_url[MAX_SIZE_BUFFER];

    sprintf(print_path, "%smy_ftp-path%s: %s", GREEN, RESET, root_path);
    sprintf(print_url, "%smy_ftp-url%s:  http://localhost:%d", GREEN, RESET, port);

    puts(print_path);
    puts(print_url);

    launch(port, root_path);

}