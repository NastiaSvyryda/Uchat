#include "uchat_server.h"

t_clients *mx_create_client(void) {
    t_clients *client = malloc(sizeof(t_clients));
    client->fd = 0;
    client->ssl = NULL;
    client->user_id = 0;
    client->token = NULL;
    client->next = NULL;
    return client;
}


void mx_delete_client(t_clients **client, int key) {
    (*client) = (*client)->first;
    t_clients *temp = (*client), *prev;
    t_clients *first = NULL;

    if (temp != NULL && temp->fd == key) {
        SSL_free(temp->ssl);
        close(temp->fd);
        mx_strdel(&temp->token);
        first = temp->next;
        while (temp != NULL) {
            temp->first = first;
            temp = temp->next;
        }
        (*client) = (*client)->next;
        free(temp);
        return;
    }
    while (temp != NULL && temp->fd != key) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
        return;
    SSL_free(temp->ssl);
    close(temp->fd);
    mx_strdel(&temp->token);
    prev->next = temp->next;
    free(temp);
    temp = NULL;
}

void mx_print_client(t_clients *clients) {
    int i = 0;
    if (clients == NULL)
        return;
    if (clients->first == NULL)
        return;;
    clients = clients->first;

    while (clients != NULL){
        mx_printstr("=====================\n");
        mx_printstr("Client fd: ");
        mx_printint(clients->fd);
        mx_printstr("\n");
        mx_printstr("=====================\n");
        mx_printstr("Client user_id: ");
        mx_printint(clients->user_id);
        mx_printstr("\n");
        mx_printstr("=====================\n");
        clients = clients->next;
        i++;
    }
    mx_printstr("=====================\n");
    mx_printstr("Count clients: ");
    mx_printint(i);
    mx_printstr("\n=====================\n");

}
