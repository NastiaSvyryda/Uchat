#include "uchat_server.h"

static void delete_message_database(t_json_data *json) {
    t_database_query *db = mx_database_query_create();

    db->model_fill_table = mx_model_message_fill_table();
    asprintf(&db->where, "%s=%d AND %s=%d",
             db->model_fill_table[0],
             json->message.message_id,
             db->model_fill_table[1],
             json->user_id);
    mx_delete_database(mx_model_message_database(), \
                        mx_model_message_name_table(), db->where);
    mx_database_query_clean(&db);

}

void mx_controller_delete_message(t_json_data *json, t_clients *client) {
    t_list *data = NULL;

    //client->user_id = json->user_id;
    json->message.client1_id = json->user_id;
    delete_message_database(json);
    data = mx_get_user_id_from_database_channels(json->message.channel_id);
    mx_send_message_to_channel(data , client, json, \
                          JS_MES_DEL_IN, JS_MES_DEL_OUT);
    mx_del_list(data, mx_list_size(data));
}
