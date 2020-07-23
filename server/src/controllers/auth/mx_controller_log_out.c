#include "uchat_server.h"

void mx_controller_log_out(t_json_data *json, t_main *main, t_clients *cur_client) {
    t_database_query *db = mx_database_query_create();

    db->model_fill_table = mx_model_user_fill_table();
    asprintf(&db->set, "%s = %s",
             db->model_fill_table[5],
             "NULL");
    asprintf(&db->where, "%s = %i",
             db->model_fill_table[0],
             json->user_id);
    mx_update_database(mx_model_user_database(), mx_model_user_name_table(), db->set, db->where);
    mx_res_js_log_out_success(cur_client);
    mx_delete_client(&main, cur_client->fd);
    mx_database_query_clean(&db);
}
