#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <pwd.h>
#include <sys/stat.h>
#include <time.h>

#include "Extra_Methods.h"
#include "Url_Methods.h"

#define MAX_SIZE_BUFFER 10240
#define HTML_TABLE_ELEMENTS "<!-- TABLE_ELEMENTS -->"

void build_name_table(char *html_response, char *path, char *root_path, char *file, struct stat st) 
{
    strcat(html_response, "<td>");
    strcat(html_response, "<a ");
    strcat(html_response, "href=\"");

    char *temp = from_path_to_browser(path, root_path);
    char *redirect = (char *) malloc(strlen(temp) + strlen(file) + 2);
    strcpy(redirect, temp);
    strcat(redirect, "/");
    strcat(redirect, file);

    char *url = to_url(redirect);

    strcat(html_response, url);
    free(redirect);
    free(temp);
    free(url);

    if (S_ISDIR(st.st_mode)) {
        strcat(html_response, "\"><span><div></div> ");
    } else {
        strcat(html_response, "\"><span><div></div> ");
    }
    strcat(html_response, file);

    strcat(html_response, "</span></a>");
    strcat(html_response, "</td>");
}

void build_size_table(char *html_response, struct stat st) 
{
    strcat(html_response, "<td>");

    char aux[64];
    if (!S_ISDIR(st.st_mode)) 
    {
        unsigned long t = st.st_size / 1024;
        char *type;

        if (t > 1024) 
        {
            t /= 1024;
            type = " mb";

            if (t > 1024) 
            {
                t /= 1024;
                type = " gb";
            }
        } 
        else 
        {
            type = " kb";
        }

        sprintf(aux, "%ld", t);
        strcat(html_response, aux);
        strcat(html_response, type);
    } 
    else 
    {
        strcat(html_response,"-");
    }

    strcat(html_response, "</td>");
}

void build_date_table(char *html_response, struct stat st) 
{
    strcat(html_response, "<td>");

    struct tm *time;

    time = localtime(&st.st_mtime);

    char temp[64];
    sprintf(temp, "%d-%02d-%02d %02d:%02d:%02d", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday,
            time->tm_hour, time->tm_min, time->tm_sec);

    strcat(html_response, temp);
    strcat(html_response, "</td>");
}

char **html_loader() 
{
    FILE *f_html = fopen("./index.html", "r");

    char buffer[MAX_SIZE_BUFFER];

    fread(buffer, MAX_SIZE_BUFFER, 1, f_html);

    char *p = strstr(buffer, HTML_TABLE_ELEMENTS);

    char **html = (char **) malloc(2 * sizeof(char *));

    html[1] = (char *) malloc(strlen(p) + 1);
    strcpy(html[1], p);

    buffer[p - buffer] = 0;
    html[0] = (char *) malloc(strlen(buffer) + 1);
    strcpy(html[0], buffer);

    fclose(f_html);

    return html;
}

void build_back(char *html_response, char *path, char *root_path) 
{
    strcat(html_response, "<td>");
    strcat(html_response, "<a href=\"");

    char *path_to_browser = from_path_to_browser(path, root_path);
    clean_last_path(path_to_browser);

    char *url = to_url(path_to_browser);
    strcat(html_response, url);
    free(path_to_browser);
    free(url);

    strcat(html_response, "\">");
    strcat(html_response, "<div></div></a></td>");
    strcat(html_response, "<td></td><td></td></tr>");
}

void cp_cat_html_tags(char *temp_path, struct dirent *directory, char *html_response, char *path, char *root_path, struct stat st)
{
    strcpy(temp_path, path);
    strcat(temp_path, "/");
    strcat(temp_path, directory->d_name);

    stat(temp_path, &st);

    strcat(html_response, "<tr>");

    build_name_table(html_response, path, root_path, directory->d_name, st);
    build_size_table(html_response, st);
    build_date_table(html_response, st);

    strcat(html_response, "</tr>");
}


char *build_html(DIR *dir_f, char *path, char *root_path) 
{
    char **html = html_loader();
    int ind = 2;
    char *html_response = (char *) malloc(MAX_SIZE_BUFFER * ind);
    strcpy(html_response, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    strcat(html_response, html[0]);

    struct stat st;
    struct dirent *directory;

    build_back(html_response, path, root_path);

    while ((directory = readdir(dir_f)) != NULL) 
    {
        if (strcmp(directory->d_name, ".") == 0) 
            continue;
        if (strcmp(directory->d_name, "..") == 0) 
            continue;
        
        int check_index;
        char *initial_point;
        
        initial_point = strchr(directory->d_name, '.');
        check_index = (int)(initial_point-directory->d_name);

        if(check_index == 0)
            continue;

        char temp_path[strlen(path) + strlen(directory->d_name) + 1];

        cp_cat_html_tags(temp_path, directory, html_response, path, root_path, st);

        if (MAX_SIZE_BUFFER * ind - strlen(html_response) < MAX_SIZE_BUFFER) {
            ind *= 2;
            html_response = (char *) realloc(html_response, MAX_SIZE_BUFFER * ind);
        }
    }

    strcat(html_response, html[1]);
    free(html);

    return html_response;
}

char *render_html(DIR *directory, char *path, char *root_path) 
{
    char *html_response = build_html(directory, path, root_path);

    return html_response;
}