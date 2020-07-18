#include "uchat_server.h"

t_clients *mx_create_client(void) {
    t_clients *client = malloc(sizeof(t_clients));
    client->fd = 0;
    client->ssl = NULL;
    client->user_id = 0;
    client->token = NULL;
    client->first = client;
    client->next = NULL;
    return client;
}

void mx_free_client(t_clients **cur_client) {
    (*cur_client)->user_id = 0;
    mx_strdel(&(*cur_client)->token);
}

void mx_delete_client(t_clients *cur_client) {
    SSL_free(cur_client->ssl);
    close(cur_client->fd);
}

