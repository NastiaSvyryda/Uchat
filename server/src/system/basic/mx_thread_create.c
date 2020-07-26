#include "uchat_server.h"

void mx_delete_token(t_clients *client) {
    t_database_query *db = mx_database_query_create();

    db->model_fill_table = mx_model_user_fill_table();
    asprintf(&db->set, "%s = %s",
             db->model_fill_table[5],
             "NULL");
    asprintf(&db->where, "%s = %i",
             db->model_fill_table[0],
             client->user_id);
    mx_update_database(mx_model_user_database(), mx_model_user_name_table(), db);
    mx_database_query_clean(&db);
}

void *main_cycle(void *newfd) {
    t_json_data *json = NULL;
    t_main *main = (t_main *)newfd;
    t_clients *cur_client = main->client;
    int len = 0;
    char *json_str = NULL;
    puts("hello");
    if (SSL_accept(main->client->ssl) == -1) {
        mx_logger("Connection", "The connection is not secure");
    } else {
        while(true) {
            len = 0;
            main->client = main->client->first;
            SSL_read(cur_client->ssl, &len, 4);
            if (len == 0) {
                mx_delete_token(cur_client);
                mx_delete_client(&main, cur_client->fd);
                break;
            }
            json_str = mx_strnew(len);
            SSL_read(cur_client->ssl, json_str, len);
            mx_logger("JSON parse:", json_str);
            mx_printstr(json_str);
            json = mx_json_parse(json_str);
            mx_routes(json, main, cur_client);
            if (json != NULL) {
                free(json);
                mx_strdel(&json_str);
            }
        }
    }
    return NULL;
}

void mx_thread_create(t_main *main, struct sockaddr_in cli) {
    pthread_t thread = NULL;

    if ((pthread_create(&thread, NULL, main_cycle, main)) != 0) {
        mx_printerr("uchat_server: thread creating error");
        exit(1);
    }
    mx_logger("Connection", inet_ntoa(cli.sin_addr));
}
