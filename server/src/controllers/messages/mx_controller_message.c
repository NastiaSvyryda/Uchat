#include "uchat_server.h"

static void fill_database_message(t_json_data *json) {
    char *fill_str = NULL;
    char **fill_table = NULL;
    char *value_str = NULL;
    fill_table = mx_model_message_fill_table();
    asprintf(&fill_str, "%s, %s, %s, %s",
             fill_table[1],
             fill_table[2],
             fill_table[3],
             fill_table[4]);
    asprintf(&value_str, "\"%d\", \"%s\", %s, \"%d\"",
             json->message.client1_id,
             json->message.text,
             "datetime('now')",
             json->message.channel_id);
    mx_create_databases(mx_model_message_database(), mx_model_message_name_table(), fill_str, value_str);
}

static void get_message_id_from_database(t_json_data *json) {
    char *where = NULL;
    char **fill = NULL;
    t_list *data = NULL;
    char *user_id = mx_itoa(json->message.channel_id);

    fill = mx_model_message_fill_table();
    asprintf(&where, "%s=%d AND %s='%s' AND %s=%d",
             fill[1],
             json->message.client1_id,
             fill[2],
             json->message.text,
             fill[4],
             json->message.channel_id);
    data = mx_read_database(mx_model_message_database(), mx_model_message_name_table(), fill[0], where);
    json->message.message_id = mx_atoi(data->data);
    mx_strdel(&user_id);
    mx_strdel(&where);
    mx_del_strarr(&fill);
    mx_del_list(data, mx_list_size(data));
}

t_list *mx_get_user_id_from_database_channels(int channel_id) {
    char *where = NULL;
    char **fill = NULL;
    t_list *data = NULL;

    fill = mx_model_user_channel_fill_table();
    asprintf(&where ,"%s=%d",
            fill[2],
            channel_id);
    data = mx_read_database(mx_model_user_channel_database(), mx_model_user_channel_name_table(), "user_id", where);
    mx_strdel(&where);
    mx_del_strarr(&fill);
    return data;
}
void mx_send_message_to_channel(t_list *data, t_clients *client, t_json_data *json, int type) {
    t_list *tmp = data;
    char *json_str = NULL;

    while (client != NULL) {
        data = tmp;
        while (data != NULL) {
            if (client->user_id != json->message.client1_id
                && client->user_id == mx_atoi(data->data)) {
                json->type = type;
                json_str = mx_json_make_json(type, json);
                SSL_write(client->ssl, json_str, mx_strlen(json_str + 4) + 4);
                mx_printstr("message delivered to ");
                mx_printint(client->user_id);
                mx_printchar('\n');
                break;
            }
            data = data->next;
        }
        client = client->next;
    }
}

void mx_controller_message(t_clients *client, t_json_data *json) {
    t_list *data = NULL;
    client = client->first;

    fill_database_message(json);
    data = mx_get_user_id_from_database_channels(json->message.channel_id);
    get_message_id_from_database(json);
    if (client->first->index > 1) { // количество пользователей
        mx_send_message_to_channel(data, client, json, JS_MES_IN);
    }
    mx_del_list(data, mx_list_size(data));
}

