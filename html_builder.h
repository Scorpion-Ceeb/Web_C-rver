#ifndef BUILD_HTML_H
#define BUILD_HTML_H

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>

void build_name_table(char *html_response, char *path, char *root_path, char *file, struct stat st);

void build_size_table(char *html_response, struct stat st);

void build_date_table(char *html_response, struct stat st);

char **html_loader();

void build_back(char *html_response, char *path, char *root_path);

void cp_cat_html_tags(char temp_path, struct dirent *directory, char *html_response, char *path, char *root_path, struct stat st);

char *build_html(DIR *d, char *path, char *root_path);

char *render_html(DIR *d, char *path, char *root_path);


#endif