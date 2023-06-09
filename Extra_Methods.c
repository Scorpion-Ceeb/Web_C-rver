#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <dirent.h>
#include <curl/curl.h>

#define MAX_BUFFER_SIZE 1024


int port_checker(char *arg_values[], int *port)
{
    if (arg_values[1] == NULL) 
        *port = 5000;
    else 
    {
        char *p;
        int q = (int) strtol(arg_values[1], &p, 10);

        if (strlen(p) == 0) 
            *port = q;
        else 
        {
            fprintf(stderr, "%s: the port is not valid\n", "ERROR");

            return 0;
        }
    }
    return 1;
}

int root_checker(char *arg_values[], struct passwd *pwd, char **root_path)
{
    if (arg_values[1] == NULL || arg_values[2] == NULL) 
    {
        *root_path = pwd->pw_dir;
    } 
    else 
    {
        DIR *directory;
        directory = opendir(arg_values[2]);

        if (directory) 
            *root_path = arg_values[2];
        else 
        {
            fprintf(stderr, "%s: the path does not exist\n", "ERROR");

            return 0;
        }
        closedir(directory);
    }
    return 1;
}

char **split_line_with_separators(char *line, char *separators) 
{
    int buffer_size = MAX_BUFFER_SIZE;
    int pos = 0;
    char **tks = malloc(buffer_size * sizeof(char *));
    char *token;

    token = strtok(line, separators);
    while (token != NULL) {
        tks[pos] = token;
        pos++;

        if (pos >= buffer_size) {
            buffer_size += MAX_BUFFER_SIZE;
            tks = realloc(tks, buffer_size * sizeof(char *));
            if (!tks) 
            {
                fprintf(stderr, "ERROR\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, separators);
    }
    tks[pos] = NULL;
    return tks;
}

void clean_last_path(char *path) 
{
    int index = (int) strlen(path) - 1;
    while (path[index] != '/') 
    {
        index--;
        if (index == -1) 
            break;
    }

    path[index + 1] = 0;
}

char *to_url(char *path) 
{
    char **splitted_path = split_line_with_separators(path, "/");

    char *url = (char *) malloc(MAX_BUFFER_SIZE);
    url[0] = 0;

    if (splitted_path[0] == NULL) 
    {
        strcat(url, "/");
    }

    for (int i = 0; splitted_path[i] != NULL; i++) {
        char *encoded;

        CURL *curl = curl_easy_init();

        encoded = curl_easy_escape(curl, splitted_path[i], (int) strlen(splitted_path[i]));

        strcat(url, "/");
        strcat(url, encoded);
        curl_easy_cleanup(curl);
        free(encoded);
    }

    free(splitted_path);

    return url;
}

char *from_url(char *path) 
{
    char *decoded;
    CURL *curl = curl_easy_init();

    int decoded_length;
    decoded = curl_easy_unescape(curl, path, (int) strlen(path), &decoded_length);
    curl_easy_cleanup(curl);

    return decoded;
}

