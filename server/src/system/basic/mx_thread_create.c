#include "uchat_server.h"

void *main_cycle(void *newfd) {
    t_json_data *json = NULL;
    t_clients *client = (t_clients *)newfd;
    t_clients *cur_client = client;
    int len = 0;
    char *json_str = NULL;

    if (SSL_accept(client->ssl) == -1)     /* do SSL-protocol accept */
        mx_printstr("The connection is not secure");
    else {
        while(true) {
            len = 0;
            client = client->first;
            SSL_read(cur_client->ssl, &len, 4);
//            len -= 4; //влада джсон надо перепроверить, поведение с длиной строки
            json_str = mx_strnew(len);
            SSL_read(cur_client->ssl, json_str, len);
            mx_logger("json", json_str);
            json = mx_json_parse(json_str);
            mx_routes(json, client, cur_client);
            if (json != NULL) {
                free(json);
                mx_strdel(&json_str);
            }
        }
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
