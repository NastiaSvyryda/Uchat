#include "uchat_server.h"

char *mx_insert_token(char **fill, int id) {
    char *token = mx_create_token(256);
    t_database_query *db = mx_database_query_create();

    asprintf(&db->set, "%s = '%s'",
             fill[5],
             token);
    asprintf(&db->where, "%s = %i",
             fill[0],
             id);
    mx_update_database(mx_model_user_database(), 
                        mx_model_user_name_table(), 
                        db->set, 
                        db->where);
    mx_database_query_clean(&db);
    return token;
}
