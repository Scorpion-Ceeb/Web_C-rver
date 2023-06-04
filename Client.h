
#ifndef Client_h
#define Client_h

struct Client{
    char *root_path;
    int socket;
};

void *client_handler(void *data);


#endif