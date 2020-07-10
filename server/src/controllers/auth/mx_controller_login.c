#include "uchat_server.h"

static char *insert_token(char **fill, char *id) {
    char *token = mx_create_token(256);
    char *set = NULL;
    char *where = NULL;

    asprintf(&set, "%s = '%s'",
             fill[5],
             token);
    asprintf(&where, "%s = %s",
             fill[0],
             id);
    mx_update_database(mx_model_user_database(), mx_model_user_name_table(), set, where);
    mx_strdel(&set);
    mx_strdel(&where);
    return token;
}

static void json_login_success(t_list *data, t_clients *client) {
    char *json_str = NULL;
    t_json_data json = {.type = JS_LOG_IN,
            .status = 200,
            .pers_info.user_id = mx_atoi(data->data)};
    mx_strcpy(json.pers_info.first_name, data->next->data);
    mx_strcpy(json.pers_info.last_name, data->next->next->data);

<<<<<<< HEAD
    mx_strcpy(json.token, client->token);
=======
    client->first->index++;
    mx_strcpy(json.token, token);
>>>>>>> 6446e11fea687c3294d9d6f3d32b2632776f596b
    json_str = mx_json_make_json(JS_LOG_IN, &json);
    mx_printchar('\n');
    mx_printstr(json_str + 4);
    mx_printchar('\n');
    mx_printint(client->name_from);
    write(client->fd, json_str , strlen(json_str + 4) + 4);
    mx_strdel(&json_str);
}

static void json_login_unauthorized(t_clients *client) {
    t_json_data json = {.type = JS_LOG_IN, .status = 401};
    char *str = mx_json_make_json(JS_LOG_IN, &json);

    write(client->fd, str, mx_strlen(str + 4) + 4);
    mx_strdel(&str);
}

static void json_login_incorrectly_filled_fields(t_clients *client) {
    t_json_data json = {.type = JS_LOG_IN, .status = 412};
    char *str = mx_json_make_json(JS_LOG_IN, &json);

    write(client->fd, str, mx_strlen(str + 4) + 4);
    mx_strdel(&str);
}

void mx_controller_login(t_json_data *json, t_clients *client) {
    char *where = NULL;
    char **fill = NULL;
    t_list *data = NULL;

    if (mx_valid_login(json) == false) {
        json_login_incorrectly_filled_fields(client);
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
        client->user_id = mx_atoi(data->data);
        client->token =  insert_token(fill, data->data);
        json_login_success(data, client);
    } else
        json_login_unauthorized(client);
    mx_del_list(data, mx_list_size(data));
    mx_strdel(&where);
    mx_del_strarr(&fill);
}
