#ifndef Extra_Methods_h
#define Extra_Methods_h


char **split_line_with_separators(char *line, char *separators);

void clean_last_path(char *path);

char *to_url(char *path);

char *from_url(char *path);

char *path_to_server(char *path, char *root_path);

char *from_path_to_browser(char *path, char *root_path);

#endif