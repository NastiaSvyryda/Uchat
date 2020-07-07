#include "uchat_server.h"

static bool validation_login(t_json_data *json) {
    int status = true;

    if (mx_strlen(json->pers_info.login) > MX_VARCHAR_LEN
        || mx_strlen(json->pers_info.login) == 0)
        status = false;
    if (mx_strlen(json->pers_info.password) > MX_VARCHAR_LEN
        || mx_strlen(json->pers_info.password) == 0)
        status = false;
    return status;
}

void mx_controller_login(t_json_data *json, t_clients *client) {
    char *where = NULL;
    char **fill = NULL;
    char *fill_table = NULL;

    if (validation_login(json) == 0) {
        write(client->fd, "fail", mx_strlen("fail"));
        return;
    }
    fill = mx_model_user_fill_table();
    asprintf(&fill_table, "%s, %s",
             fill[2],
             fill[3]);
    asprintf(&where, "%s = '%s'",
             fill[2],
             json->pers_info.login);
    mx_read_database(mx_model_user_database(), mx_model_user_name_table(), fill_table, where);
    write(client->fd, "success", mx_strlen("success"));
    mx_strdel(&where);
    mx_strdel(&fill_table);
    mx_del_strarr(&fill);
}
