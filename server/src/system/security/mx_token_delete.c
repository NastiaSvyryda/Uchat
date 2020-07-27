#include "uchat_server.h"

void mx_token_delete(t_clients *client) {
    t_database_query *db = mx_database_query_create();

    db->model_fill_table = mx_model_user_fill_table();
    asprintf(&db->set, "%s = %s",
             db->model_fill_table[5],
             "NULL");
    asprintf(&db->where, "%s = %i",
             db->model_fill_table[0],
             client->user_id);
    mx_update_database(mx_model_user_database(), mx_model_user_name_table(), db);
    mx_database_query_clean(&db);
}
