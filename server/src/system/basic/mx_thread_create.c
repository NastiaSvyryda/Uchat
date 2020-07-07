#include "uchat_server.h"

void *main_cycle(void *newfd) {
    t_json_data *json = NULL;
    t_clients *client = (t_clients *)newfd;
    int curr_fd = client->fd;
    t_clients *cur_client = client;
    char lin[4];
    int len = 0;
    char *json_str = NULL;

    while(true) {
//        if (client->first->index > 1) { // количество пользователей
            client = client->first;
            read(curr_fd, lin, 4);
            len = lin[0];
            json_str = mx_strnew(len);
            read(curr_fd, json_str, len);
            json = mx_json_parse(json_str);
            mx_routes(json, client, cur_client);
            mx_strdel(&json->message.text);
            free(json);
        }
//    }
    return NULL;
}

void mx_thread_create(t_clients *client, struct sockaddr_in cli) {
    pthread_t thread = NULL;

    if ((pthread_create(&thread, NULL, main_cycle, client)) != 0) {
        mx_printerr("uchat_server: thread creating error");
        exit(1);
    }
    printf("%s ", "\033[0;32mUser connected from ip:\033[0;32m");
    printf("%s\n", inet_ntoa(cli.sin_addr));
}
