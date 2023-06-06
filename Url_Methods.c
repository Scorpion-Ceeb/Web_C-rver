#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "Url_Methods.h"
#include "Extra_Methods.h"




char *path_to_server(char *path, char *root_path) 
{
    char *path_decoded = from_url(path);

    int length_path_decoded = (int) strlen(path_decoded);
    int lenght_root_path = (int) strlen(root_path);

    char *new_path = (char *) malloc(lenght_root_path + length_path_decoded + 1);
    strcpy(new_path, root_path);
    strcat(new_path, path_decoded);

    if (new_path[length_path_decoded + lenght_root_path - 1] == '/')
    {

        new_path[length_path_decoded + lenght_root_path - 1] = 0;
    }

    free(path_decoded);

    return new_path;
}

char *from_path_to_browser(char *path, char *root_path) 
{
    int index;
    int length_path = (int) strlen(path);
    int length_root_path = (int) strlen(root_path);

    char *new_path = (char *) malloc(length_path + 1);

    for (index = 0; index < length_path - length_root_path; index++) {
        new_path[index] = path[index + length_root_path];
    }
    new_path[index] = 0;

    return new_path;
}