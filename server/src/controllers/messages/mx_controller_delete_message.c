#include "uchat_server.h"

static void delete_message_database(t_json_data *json) {
    char *where = NULL;
    char **fill = NULL;

    fill = mx_model_message_fill_table();
    asprintf(&where, "%s=%d AND %s=%d",
             fill[0],
             json->message.message_id,
             fill[1],
             json->user_id);
    mx_delete_database(mx_model_message_database(), mx_model_message_name_table(), where);
    mx_strdel(&where);
    mx_del_strarr(&fill);
}

void mx_controller_delete_message(t_json_data *json, t_main *main) {
    t_list *data = NULL;
    main->client->user_id = json->user_id;
    json->message.client1_id = json->user_id;
    delete_message_database(json);
    data = mx_get_user_id_from_database_channels(json->message.channel_id);
    mx_send_message_to_channel(data ,main, json, JS_MES_DEL_IN, JS_MES_DEL_OUT);
    mx_del_list(data, mx_list_size(data));
}
