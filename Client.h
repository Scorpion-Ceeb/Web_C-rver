
#ifndef Client_h
#define Client_h

struct Client
{
    int socket;
    char *root_path;
};

void *client_handler(void *client_data);


#endif