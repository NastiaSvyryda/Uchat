#include "uchat_server.h"

bool mx_valid_is_login(char *model_fill_table, t_json_data *json) {
    t_database_query *db = mx_database_query_create();
    bool status = true;

    db->fill_table = mx_strdup(model_fill_table);
    asprintf(&db->where, "%s = '%s'",
             model_fill_table,
             json->pers_info.login);
    db->list = mx_read_database(mx_model_user_database(), \
                                mx_model_user_name_table(), db);
    if(db->list == NULL)
        status = false;
    mx_database_query_clean(&db);
    return status;
}
