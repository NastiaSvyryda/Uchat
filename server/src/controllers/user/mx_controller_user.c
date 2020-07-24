#include "uchat_server.h"

void mx_controller_user_all(t_clients *client) {
    t_database_query *db = mx_database_query_create();

    db->model_fill_table = mx_model_user_fill_table();
    asprintf(&db->fill_table, "%s, %s",
             db->model_fill_table[0],
             db->model_fill_table[3]);
    db->where = NULL;
    db->list = mx_read_database(mx_model_user_database(), mx_model_user_name_table(), db);
    mx_res_js_get_users(client, db->list);
    mx_database_query_clean(&db);
}
