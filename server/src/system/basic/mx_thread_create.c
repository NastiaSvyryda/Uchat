#include "uchat_server.h"

void *main_cycle(void *newfd) {
    t_json_data *json = NULL;
    t_clients *client = (t_clients *)newfd;
    int curr_fd = client->fd;
    t_clients *cur_client = client;
    char len_str[4];
    int len = 0;
//    char *json_str;
    char *json_str = NULL;

    while(true) {
            client = client->first;
            read(curr_fd, len_str, 4);
            for (int i = 0; i < 4; i++)
                len += len_str[i];
            json_str = mx_strnew(len);
            read(curr_fd, json_str, len);
            mx_printstr(json_str);
            json = mx_json_parse(json_str);
            mx_routes(json, client, cur_client);
            if (json != NULL) {
//                mx_strdel(&json->message.text);
                free(json);
                mx_strdel(&json_str);
                memset(len_str, '\0', 4);
            }
//        }
    }
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
