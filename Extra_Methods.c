#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

#define MAX_BUFFER_SIZE 1024

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