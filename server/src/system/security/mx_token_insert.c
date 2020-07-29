#include "uchat_server.h"

char *mx_token_insert(char **fill, int id) {
    char *token = mx_token_create(256);
    t_database_query *db = mx_database_query_create();

    asprintf(&db->set, "%s = '%s'",
             fill[5],
             token);
    asprintf(&db->where, "%s = %i",
             fill[0],
             id);
    mx_update_database(mx_model_user_database(), \
                        mx_model_user_name_table(), db);
    mx_database_query_clean(&db);
    return token;
}
