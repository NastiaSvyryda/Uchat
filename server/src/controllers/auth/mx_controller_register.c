#include "uchat_server.h"

static void json_register_incorrectly_filled_fields(t_clients *client) {
    t_json_data json = {.type = JS_REG, .status = 412, .pers_info.user_id = 0, .pers_info.first_name = "", .pers_info.last_name = ""};
    char *str = mx_json_make_json(JS_LOG_IN, &json);

    write(client->fd, str, mx_strlen(str + 4) + 4);
    mx_strdel(&str);
}

static void json_register_success(t_clients *client, t_json_data *json_data) {
    t_json_data json = {.type = JS_REG, .status = 200, .pers_info.user_id = 1};
    strcpy(json.pers_info.first_name, json_data->pers_info.first_name);
    strcpy(json.pers_info.last_name, json_data->pers_info.last_name);
    strcpy(json.token, "hui");
    char *str = mx_json_make_json(JS_REG, &json);
     mx_printstr(str + 4);
    write(client->fd, str, mx_strlen(str + 4) + 4);
    mx_strdel(&str);
}

static t_list *check_login (char **fill, t_json_data *json) {
    char *where = NULL;

    asprintf(&where, "login = '%s'", json->pers_info.login);
    return mx_read_database(mx_model_user_database(), mx_model_user_name_table(), fill[3], where);
}


void mx_controller_register(t_json_data *json, t_clients *client) {
    char *value_table = NULL;
    char **fill = NULL;
    char *fill_table = NULL;
    t_list *data_check = NULL;

    if (mx_valid_register(json) == false) {
        json_register_incorrectly_filled_fields(client);
        return;
    }

    fill = mx_model_user_fill_table();
    asprintf(&fill_table, "%s, %s, %s, %s, %s",
             fill[1],
             fill[2],
             fill[3],
             fill[4],
             fill[6]);
    data_check = check_login(fill, json);
    if (data_check == NULL) {
        asprintf(&value_table, "\"%s\", \"%s\", \"%s\", \"%s\", %s",
                 json->pers_info.first_name,
                 json->pers_info.last_name,
                 json->pers_info.login,
                 json->pers_info.password,
                 "datetime('now')");
        mx_create_databases(mx_model_user_database(), mx_model_user_name_table(), fill_table, value_table);
        json_register_success(client, json);
    } else
        json_register_incorrectly_filled_fields(client);
    mx_strdel(&fill_table);
    mx_del_strarr(&fill);
    mx_strdel(&value_table);
    mx_del_list(data_check, mx_list_size(data_check));
}
