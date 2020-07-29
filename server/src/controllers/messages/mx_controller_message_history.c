#include "uchat_server.h"

void mx_controller_message_history(t_json_data *json, t_clients *client) {
    t_database_query *db = mx_database_query_create();

    db->model_fill_table = mx_model_message_fill_table();
    asprintf(&db->fill_table, "%s", "*");
    if (json->message.last_message_id == -1) {
        asprintf(&db->where, "%s=%i ORDER BY %s DESC LIMIT %i",
                 db->model_fill_table[4],
                 json->message.channel_id,
                 db->model_fill_table[0],
                 mx_config_pagination() );
    } else {
        asprintf(&db->where, "%s=%i ORDER BY %s DESC LIMIT %i OFFSET %i",
                 db->model_fill_table[4], json->message.channel_id,
                 db->model_fill_table[0], json->message.last_message_id,
                 json->message.last_message_id + mx_config_pagination());
    }
    db->list = mx_read_database(mx_model_message_database(), \
    mx_model_message_name_table(), db);
    mx_res_js_get_message_history(client, db->list, json->message.channel_id);
    mx_database_query_clean(&db);
}
