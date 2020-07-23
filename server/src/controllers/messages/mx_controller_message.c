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
    mx_del_strarr(&fill_table);
    mx_strdel(&value_str);
    mx_strdel(&fill_str);
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
    mx_del_list(data, mx_list_size(data));
    data = mx_read_database(mx_model_message_database(), mx_model_message_name_table(), fill[3], where);
    json->message.delivery_time = (long)data->data;
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
void mx_send_message_to_channel(t_list *data, t_main *main, t_json_data *json, int type, int type_response) {
    t_list *tmp = data;
    char *json_str = NULL;
    int i = 0;

        while (main->client != NULL) {
            data = tmp;
            while (data != NULL) {
                if (main->client->user_id != json->message.client1_id
                    && main->client->user_id == mx_atoi(data->data)) {
                    json->type = type;
                    json_str = mx_json_make_json(type, json);
                    mx_logger("JSON write:", json_str + 4);
                    SSL_write(main->client->ssl, json_str, mx_strlen(json_str + 4) + 4);
                    break;
                } else if (main->client->user_id == json->message.client1_id
                           && main->client->user_id == mx_atoi(data->data)) {
                    json->type = type_response;
                    json->status = 200;
                    json_str = mx_json_make_json(type_response, json);
                    mx_logger("JSON write:", json_str + 4);
                    SSL_write(main->client->ssl, json_str, mx_strlen(json_str + 4) + 4);
                    i++;
                }
//                else if (client->user_id != json->message.client1_id
//                         && client->user_id != mx_atoi(data->data)) {
                    else if (mx_atoi(data->data) != json->message.client1_id && main->client->user_id != mx_atoi(data->data) &&  i < mx_list_size(data) ){
                    json->type = type;
                    main->wait->json_str = mx_json_make_json(type, json);
                    mx_printstr("wait:\n");
                    mx_printint(mx_atoi(data->data));
                    mx_printstr(main->wait->json_str+4);
                    main->wait->user_id = mx_atoi(data->data);
//                    client->first->wait->first = client->first->wait;
                    main->wait->next = malloc(sizeof(t_wait));
                    main->wait->next->json_str = NULL;
                    main->wait->next->first = main->wait->first;
                    main->wait = main->wait->next;
                    main->wait->next = NULL;
                    i++;
                }
                data = data->next;
            }
            if(main->client->next != NULL)
                main->client = main->client->next;
            else
                break;
        }
}

void mx_controller_message(t_main *main, t_json_data *json) {
    t_list *data = NULL;
    main->client = main->client->first;
    if (json->new_channel == true) {
        mx_controller_new_channel(json);
    }
    fill_database_message(json);
    data = mx_get_user_id_from_database_channels(json->message.channel_id);
    get_message_id_from_database(json);
    mx_send_message_to_channel(data, main, json, JS_MES_IN, JS_MES_OUT);
    mx_del_list(data, mx_list_size(data));
}

