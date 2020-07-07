#include "uchat_server.h"

static int validation_register(t_json_data *json) {
    int status = true;

    if (mx_strlen(json->pers_info.login) > MX_VARCHAR_LEN
        || mx_strlen(json->pers_info.login) == 0)
        status = false;
    if (mx_strlen(json->pers_info.password) > MX_VARCHAR_LEN
        || mx_strlen(json->pers_info.password) == 0)
        status = false;
    if (mx_strlen(json->pers_info.first_name) > MX_MAX_NAME_LEN
        || mx_strlen(json->pers_info.first_name) == 0)
        status = false;
    if (mx_strlen(json->pers_info.last_name) > MX_MAX_NAME_LEN
        || mx_strlen(json->pers_info.first_name) == 0)
        status = false;
    return status;
}

void mx_controller_register(t_json_data *json, t_clients *client) {
    char *value_table = NULL;
    char **fill = NULL;
    char *fill_table = NULL;

    if (validation_register(json) == false) {
        write(client->fd, "fail", mx_strlen("fail"));
        return;
    }
    fill = mx_model_user_fill_table();
    asprintf(&fill_table, "%s, %s, %s, %s, %s",
             fill[0],
             fill[1],
             fill[2],
             fill[3],
             fill[5]);
    asprintf(&value_table, "\"%s\", \"%s\", \"%s\", \"%s\", %s",
             json->pers_info.first_name,
             json->pers_info.last_name,
             json->pers_info.login,
             json->pers_info.password,
             "datetime('now')");
    mx_create_databases(mx_model_user_database(), mx_model_user_name_table(), fill_table, value_table);
    write(client->fd, "success", mx_strlen("success"));
    mx_strdel(&fill_table);
    mx_del_strarr(&fill);
    mx_strdel(&value_table);
}

