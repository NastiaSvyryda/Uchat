#include "uchat_server.h"

static void change_message_database(t_json_data *json) {
    char *set = NULL;
    char *where = NULL;
    char **fill = NULL;

    fill = mx_model_message_fill_table();
    asprintf(&set, "%s='%s'",
            fill[2],
            json->message.text);
    asprintf(&where, "%s=%d AND %s=%d",
            fill[0],
            json->message.message_id,
            fill[1],
            json->message.client1_id);

    mx_update_database(mx_model_message_database(), mx_model_message_name_table(), set, where);
    mx_strdel(&set);
    mx_strdel(&where);
    mx_del_strarr(&fill);
}

static void get_channel_id_from_database(t_json_data *json, t_json_data *json_response) {
    char *where = NULL;
    char **fill = NULL;
    t_list *data = NULL;
    char *user_id = mx_itoa(json->message.channel_id);

    fill = mx_model_message_fill_table();
    asprintf(&where, "%s=%d AND %s=%d",
             fill[0],
             json->message.message_id,
             fill[1],
             json->message.client1_id);
    data = mx_read_database(mx_model_message_database(), mx_model_message_name_table(), "channel_id", where);
    json_response->message.channel_id = mx_atoi(data->data);
    mx_strdel(&user_id);
    mx_strdel(&where);
    mx_del_strarr(&fill);
    mx_del_list(data, mx_list_size(data));
}



void mx_controller_edit_message(t_json_data *json, t_clients *client) {
    t_list *data = NULL;
    t_json_data json_response = {.type = JS_MES_EDIT_IN, .message.channel_id = json->message.channel_id,
                                 .message.message_id = json->message.message_id, .message.client1_id = json->message.client1_id};

    json_response.message.text = mx_strdup(json->message.text);
    client = client->first;
    change_message_database(json);
    get_channel_id_from_database(json, &json_response);
    data = mx_get_user_id_from_database_channels(json_response.message.channel_id);
    mx_send_message_to_channel(data, client, &json_response, JS_MES_EDIT_IN);
    mx_strdel(&json_response.message.text);

}
