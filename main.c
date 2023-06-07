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
#include "Extra_Methods.h"

#define MAX_SIZE_BUFFER 1024
#define BBlue "\033[1;34m"
#define RESET "\033[0m"



int main(int argn, char *arg_values[]) 
{

    int port;
    char *root_path;

    uid_t uid;

    uid = getuid();
    struct passwd *pwd = getpwuid(uid);

    int passed_port = port_checker(arg_values, &port);
    
    if (!passed_port)
        return 0;

    int passed_root= root_checker(arg_values, pwd, &root_path);

    if(!passed_root)
        return 0;

    char modified_path_to_print[MAX_SIZE_BUFFER];
    char modified_url_to_print[MAX_SIZE_BUFFER];

    sprintf(modified_path_to_print, "%sPath%s: %s", BBlue, RESET, root_path);
    sprintf(modified_url_to_print, "%sURL%s:  http://localhost:%d", BBlue, RESET, port);

    puts(modified_path_to_print);
    puts(modified_url_to_print);


    launch(port, root_path);
}