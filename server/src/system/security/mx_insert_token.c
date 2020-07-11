#include "uchat_server.h"

char *mx_insert_token(char **fill, int id) {
    char *token = mx_create_token(256);
    char *set = NULL;
    char *where = NULL;

    asprintf(&set, "%s = '%s'",
             fill[5],
             token);
    asprintf(&where, "%s = %i",
             fill[0],
             id);
    mx_update_database(mx_model_user_database(), mx_model_user_name_table(), set, where);
    mx_strdel(&set);
    mx_strdel(&where);
    return token;
}
