#include "uchat_server.h"

static bool validation_login(t_json_data *json) {
    int status = true;

    if ((mx_strlen(json->pers_info.login) == 0)
        || (mx_strlen(json->pers_info.login) > MX_VARCHAR_LEN)
        || (mx_valid_str_isalpha(json->pers_info.login) == false))
        status = false;
    if ((mx_strlen(json->pers_info.password) == 0)
        || (mx_strlen(json->pers_info.password) > MX_VARCHAR_LEN)
        || (mx_valid_str_isalpha(json->pers_info.password) == false))
        status = false;
    return status;
}

static char *insert_token(char **fill, t_list *data) {
    char *token = mx_create_token(256);
    char *set = NULL;
    char *where = NULL;

    asprintf(&set, "%s = '%s'",
             fill[5],
             token);
    asprintf(&where, "%s = %s",
             fill[0],
             data->data);
    mx_update_database(mx_model_user_database(), mx_model_user_name_table(), set, where);
    mx_strdel(&set);
    mx_strdel(&where);
    return token;
}

static void login_success(t_list *data, char *token, t_clients *client) {
    t_json_data json = {.type = JS_LOG_IN,
            .status = 200,
            .user_id = mx_atoi(data->data)};
    mx_strcpy(json.pers_info.first_name, data->next->data);
    mx_strcpy(json.pers_info.last_name, data->next->next->data);
    mx_strcpy(json.token, token);
    char *str = mx_json_make_json(JS_LOG_IN, &json);
    write(client->fd, str + 4, mx_strlen(str + 4));
}

static void login_unauthorized(t_clients *client) {
    t_json_data json = {.type = JS_LOG_IN, .status = 401};
    char *str = mx_json_make_json(JS_LOG_IN, &json);

    write(client->fd, str + 4, mx_strlen(str + 4));
    mx_strdel(&str);
}

static void login_incorrectly_filled_fields(t_clients *client) {
    t_json_data json = {.type = JS_LOG_IN, .status = 412};
    char *str = mx_json_make_json(JS_LOG_IN, &json);

    write(client->fd, str + 4, mx_strlen(str + 4));
    mx_strdel(&str);
}

void mx_controller_login(t_json_data *json, t_clients *client) {
    char *where = NULL;
    char **fill = NULL;
    t_list *data = NULL;

    if (validation_login(json) == false) {
        login_incorrectly_filled_fields(client);
        return;
    }
    fill = mx_model_user_fill_table();
    asprintf(&where, "%s='%s' AND %s='%s'",
             fill[3],
             json->pers_info.login,
             fill[4],
             json->pers_info.password);
    data = mx_read_database(mx_model_user_database(), mx_model_user_name_table(), "*", where);
    if (data != NULL) {
        client->name_from = mx_atoi(data->data);
        char *token =  insert_token(fill, data);
        login_success(data, token, client);
    } else
        login_unauthorized(client);
    mx_del_list(data, mx_list_size(data));
    mx_strdel(&where);
    mx_del_strarr(&fill);
}
