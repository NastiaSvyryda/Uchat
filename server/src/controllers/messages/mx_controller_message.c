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

static t_list *get_from_database(t_json_data *json) {
    char *where = NULL;
    char **fill = NULL;
    t_list *data = NULL;
    char *user_id = mx_itoa(json->message.channel_id);

    fill = mx_model_user_channel_fill_table();
    asprintf(&where ,"%s='%d'",
            fill[2],
            json->message.channel_id);
    data = mx_read_database(mx_model_user_channel_database(), mx_model_user_channel_name_table(), "user_id", where);
    mx_strdel(&user_id);
    mx_strdel(&where);
    mx_del_strarr(&fill);
//    mx_del_list(data, mx_list_size(data));
    return data;
}

void mx_controller_message(t_clients *client, t_json_data *json) {
    char *json_str = NULL;
    t_list *data = NULL;
    client = client->first;
    t_list *tmp = NULL;

    fill_database_message(json);
    data = get_from_database(json);
    tmp = data;
    if (client->first->index > 1) { // количество пользователей
            while (client != NULL) {
                data = tmp;
                while(data != NULL) {
                    if (client->user_id != json->message.client1_id
                        && client->user_id == mx_atoi(data->data)) {
                        json->type = JS_MES_IN;
                        json_str = mx_json_make_json(JS_MES_IN, json);
                        write(client->fd, json_str, mx_strlen(json_str + 4) + 4);
                        mx_printstr("message delivered to ");
                        mx_printint(client->channel_id);
                        mx_printchar('\n');
                        break;
                    }
                    data = data->next;
                }
                client = client->next;
        }
    }
    mx_del_list(data, mx_list_size(data));
}

