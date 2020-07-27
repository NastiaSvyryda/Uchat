#include "uchat_server.h"

bool mx_valid_token(int id, char *token) {
    t_database_query *db = mx_database_query_create();
    bool status = true;

    db->model_fill_table = mx_model_user_fill_table();
    asprintf(&db->fill_table ,"%s, %s",
            db->model_fill_table[0],
            db->model_fill_table[5]);
    asprintf(&db->where ,"%s=%i AND %s='%s'",
            db->model_fill_table[0],
            id,
            db->model_fill_table[5],
            token);
    db->list = mx_read_database(mx_model_user_database(), mx_model_user_name_table(), db);
    if (db->list == NULL)
        status = false;
    mx_database_query_clean(&db);
    return status;
}
