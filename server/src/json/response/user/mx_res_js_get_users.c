#include "uchat_server.h"

void mx_res_js_get_users(t_clients *client, t_list *list) {
    t_json_data json = {.type = JS_GET_USERS, .status = 200};
    char *new_json = NULL;
    int len = mx_list_size(list);

    json.ids_logins_arr = malloc(sizeof(t_id_login) * len/2);
    for (int i = 0; list != NULL; i++) {
        json.ids_logins_arr[i].user_id = mx_atoi(list->data);
        strcpy(json.ids_logins_arr[i].login, list->next->data);
        list = list->next->next;
    }
    json.ids_logins_arr_size = len / 2;
    new_json = mx_json_make_json(JS_GET_USERS, &json);
    mx_logger("JSON write:",  new_json + 4);
    mx_printstr(new_json + 4);
    SSL_write(client->ssl, new_json, mx_strlen(new_json + 4) + 4);
    free(json.ids_logins_arr);
    mx_strdel(&new_json);
}
