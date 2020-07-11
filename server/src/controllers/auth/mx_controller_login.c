#include "uchat_server.h"

static void json_login_success(t_list *data, t_clients *client) {
    char *new_json = NULL;

    t_json_data json = {.type = JS_LOG_IN,
            .status = 200,
            .pers_info.user_id = mx_atoi(data->data)};
    mx_strcpy(json.pers_info.first_name, data->next->data);
    mx_strcpy(json.pers_info.last_name, data->next->next->data);
    mx_strcpy(json.token, client->token);
    client->first->index++;
    new_json = mx_json_make_json(JS_LOG_IN, &json);
    write(client->fd, new_json , strlen(new_json + 4) + 4);
    mx_strdel(&new_json);
}

static void json_login_unauthorized(t_clients *client) {
    t_json_data json = {.type = JS_LOG_IN, .status = 401};
    char *new_json = mx_json_make_json(JS_LOG_IN, &json);

    write(client->fd, new_json, mx_strlen(new_json + 4) + 4);
    mx_strdel(&new_json);
}

static void json_login_incorrectly_filled_fields(t_clients *client) {
    t_json_data json = {.type = JS_LOG_IN, .status = 412};
    char *new_json = mx_json_make_json(JS_LOG_IN, &json);

    write(client->fd, new_json, mx_strlen(new_json + 4) + 4);
    mx_strdel(&new_json);
}

void mx_controller_login(t_json_data *json, t_clients *client) {
    char *where = NULL;
    char **fill = NULL;
    t_list *data = NULL;
    char *password = NULL;

    if (mx_valid_login(json) == false) {
        json_login_incorrectly_filled_fields(client);
        return;
    }
    fill = mx_model_user_fill_table();
    password = mx_hmac_sha_256(json->pers_info.login, json->pers_info.password);
    asprintf(&where, "%s='%s' AND %s='%s'",
             fill[3],
             json->pers_info.login,
             fill[4],
             password);
    data = mx_read_database(mx_model_user_database(), mx_model_user_name_table(), "*", where);
    if (data != NULL) {
        client->user_id = mx_atoi(data->data);
        client->token =  mx_insert_token(fill, client->user_id);
        json_login_success(data, client);
    } else
        json_login_unauthorized(client);
    mx_del_list(data, mx_list_size(data));
    mx_strdel(&where);
    mx_strdel(&password);
    mx_del_strarr(&fill);
}
