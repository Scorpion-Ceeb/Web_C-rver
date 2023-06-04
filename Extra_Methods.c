
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define MAX_BUFFER_SIZE 1024


char **split_line_with_separators(char *line, char *separators) 
{
    int pos = 0;
    int buffer_size = MAX_BUFFER_SIZE;
    char **tks = malloc(buffer_size * sizeof(char *));
    char *token;

    token = strtok(line, separators);

    while (token != NULL) {
        tks[pos] = token;
        pos++;

        if (pos >= buffer_size) {
            buffer_size += MAX_BUFFER_SIZE;
            tks = realloc(tks, buffer_size * sizeof(char *));
            if (!tks) {
                fprintf(stderr, "Error\n");
                exit(1);
            }
        }

        token = strtok(NULL, separators);
    }
    tks[pos] = NULL;
    return tks;
}

void clean_last_path(char *path) 
{
    int p = (int) strlen(path) - 1;
    while (path[p] != '/') {
        p--;
        if (p == -1) break;
    }

    path[p + 1] = 0;
}

char *to_url(char *path) 
{
    char **values_url = split_line_with_separators(path, "/");

    char *url = (char *) malloc(MAX_BUFFER_SIZE);
    url[0] = 0;

    if (values_url[0] == NULL) 
        strcat(url, "/");

    for (int i = 0; values_url[i] != NULL; i++) {
        char *temp_url;

        CURL *curl = curl_easy_init();

        temp_url = curl_easy_escape(curl, values_url[i], (int) strlen(values_url[i]));

        strcat(url, "/");
        strcat(url, temp_url);
        curl_easy_cleanup(curl);
        free(temp_url);
    }

    free(values_url);

    return url;
}

char *from_url(char *path) 
{
    char *url_temp;
    CURL *curl = curl_easy_init();

    int url_temp_length;
    url_temp = curl_easy_unescape(curl, path, (int) strlen(path), &url_temp_length);
    curl_easy_cleanup(curl);

    return url_temp;
}

char *path_to_server(char *path, char *root_path) 
{
    char *url_path = from_url(path);

    int len = (int) strlen(url_path);
    int len_root_path = (int) strlen(root_path);

    char *new_path = (char *) malloc(len_root_path + len + 1);
    strcpy(new_path, root_path);
    strcat(new_path, url_path);

    if (new_path[len + len_root_path - 1] == '/') new_path[len + len_root_path - 1] = 0;

    free(url_path);
    return new_path;
}

char *from_path_to_browser(char *path, char *root_path) 
{
    int i;
    int len = (int) strlen(path);
    int len_root_path = (int) strlen(root_path);

    char *new_path = (char *) malloc(len + 1);

    for (i = 0; i < len - len_root_path; i++) {
        new_path[i] = path[i + len_root_path];
    }
    new_path[i] = 0;

    return new_path;
}