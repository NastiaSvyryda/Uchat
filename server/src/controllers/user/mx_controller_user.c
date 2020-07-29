#include "uchat_server.h"

void mx_controller_user_all(t_clients *client) {
    t_database_query *db = mx_database_query_create();

    db->model_fill_table = mx_model_user_fill_table();
    asprintf(&db->fill_table, "%s, %s",
             db->model_fill_table[0],
             db->model_fill_table[3]);
    db->where = NULL;
    db->list = mx_read_database(mx_model_user_database(),
               mx_model_user_name_table(), db);
    mx_res_js_get_users(client, db->list);
    mx_database_query_clean(&db);
}

void mx_controller_user_edit(t_json_data *json, t_clients *client) {
    t_database_query *db = mx_database_query_create();

    db->model_fill_table = mx_model_user_fill_table();
    asprintf(&db->where, "%s = %i",
             db->model_fill_table[0], json->user_id);
    if (mx_strlen(json->pers_info.password) == 0) {
        asprintf(&db->set, "%s = '%s', %s = '%s'", db->model_fill_table[1],
                 json->pers_info.first_name, db->model_fill_table[2],
                 json->pers_info.last_name);
    }
    else {
        asprintf(&db->set, "%s = '%s', %s = '%s', %s = '%s'",
                 db->model_fill_table[1], json->pers_info.first_name,
                 db->model_fill_table[2], json->pers_info.last_name,
                 db->model_fill_table[4],
                 mx_hmac_sha_256(json->pers_info.login,json->pers_info.password));
    }
    mx_update_database(mx_model_user_database(), mx_model_user_name_table(), db);
    mx_res_js_user_edit_success(client);
    mx_database_query_clean(&db);
}
