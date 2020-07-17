#include "uchat_server.h"

bool mx_valid_token(int id, char *token) {
    t_list *data = NULL;
    char *where = NULL;
    char *fill_table = NULL;
    char **fill = NULL;
    bool status = true;

    fill = mx_model_user_fill_table();
    asprintf(&fill_table ,"%s, %s",
             fill[0],
             fill[5]);
    asprintf(&where ,"%s=%i AND %s='%s'",
            fill[0],
            id,
            fill[5],
            token);
    data = mx_read_database(mx_model_user_database(), mx_model_user_name_table(), fill_table, where);
    if (data == NULL)
        status = false;
    mx_del_list(data, mx_list_size(data));
    mx_strdel(&fill_table);
    mx_strdel(&where);
    return status;
}
