#ifndef Extra_Methods_h
#define Extra_Methods_h


char **split_line_with_separators(char *line, char *separators);

void clean_last_path(char *path);

char *to_url(char *path);

char *from_url(char *path);

#endif