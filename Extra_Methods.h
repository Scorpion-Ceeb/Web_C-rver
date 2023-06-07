#ifndef Extra_Methods_h

#include <pwd.h>

#define Extra_Methods_h

int port_checker(char *arg_values[], int *port);

int root_checker(char *arg_values[], struct passwd *pwd, char **root_path);

char **split_line_with_separators(char *line, char *separators);

void clean_last_path(char *path);

char *to_url(char *path);

char *from_url(char *path);


#endif