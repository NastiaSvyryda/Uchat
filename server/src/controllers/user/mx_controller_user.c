#include "uchat_server.h"

static void json_get_users_response(t_clients *client, t_list **list) {
    t_json_data json = {.type = JS_GET_USERS, .status = 200};
    char *new_json = NULL;
    int len = mx_list_size(*list);

    for (int i = 0; *list != NULL; i++) {
        json.ids_logins_arr[i].user_id = mx_atoi((*list)->data);
        mx_strcpy(json.ids_logins_arr[i].login, (*list)->next->next->data);
        *list = (*list)->next->next;
    }
    json.ids_logins_arr_size = len / 2;
    new_json = mx_json_make_json(JS_GET_USERS, &json);
    mx_logger("JSON write:",  new_json + 4);
    SSL_write(client->ssl, new_json, mx_strlen(new_json + 4) + 4);
    mx_del_list(*list, len);
    mx_strdel(&new_json);
}

void mx_controller_user_all(t_clients *client) {
    t_list *list = NULL;
    char *fill_table = NULL;
    char **fill = mx_model_user_fill_table();

    asprintf(&fill_table, "%s, %s",
             fill[0],
             fill[3]);
    list = mx_read_database(mx_model_user_database(), mx_model_user_name_table(), fill_table, NULL);
    json_get_users_response(client, &list);
    mx_strdel(&fill_table);
    mx_del_strarr(&fill);
}
