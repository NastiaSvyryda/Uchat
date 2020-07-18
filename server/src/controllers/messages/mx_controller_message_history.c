#include "uchat_server.h"

static void json_get_message_history(t_clients *client, t_list **list) {
    t_json_data json = {.type = JS_MES_HIST, .status = 200};
    char *new_json = NULL;
    int len = mx_list_size(*list);

    json.messages_arr = malloc(sizeof(t_message) * len / 5);
    for (int i = 0; *list != NULL; i++) {
        json.messages_arr->message_id = mx_atoi((*list)->data);
        json.messages_arr->client1_id = mx_atoi((*list)->next->data);
        mx_strcpy(json.messages_arr[i].text, ((*list)->next->next->data));
        json.messages_arr->delivery_time = mx_atoi((*list)->next->next->data);
        json.messages_arr->channel_id = mx_atoi((*list)->next->next->next->data);
        *list = (*list)->next->next->next->next;
    }
    json.messages_arr_size = len / 5;
    new_json = mx_json_make_json(JS_MES_HIST, &json);
    mx_logger("JSON write:",  new_json + 4);
    SSL_write(client->ssl, new_json, mx_strlen(new_json + 4) + 4);
    free(json.messages_arr);
    mx_del_list(*list, len);
    mx_strdel(&new_json);
}

void mx_controller_message_history(t_json_data *json, t_clients *client) {
    t_list *list = NULL;
    char *where = NULL;
    char **fill = NULL;


    fill = mx_model_message_fill_table();
    if (json->message.last_message_id == -1) {
        asprintf(&where, "%s=%i ORDER BY %s DESC LIMIT %i",
                 fill[4],
                 json->message.channel_id,
                 fill[0],
                 mx_config_pagination() );
    } else {
        asprintf(&where, "%s=%i ORDER BY %s DESC LIMIT %i OFFSET %i",
                 fill[4],
                 json->message.channel_id,
                 fill[0],
                 json->message.last_message_id,
                 json->message.last_message_id + mx_config_pagination() );
    }
    list = mx_read_database(mx_model_user_database(), mx_model_user_name_table(), "*", where);
    json_get_message_history(client, &list);
    mx_strdel(&where);
    mx_del_strarr(&fill);
}
