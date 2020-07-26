#include "uchat_server.h"

void mx_res_js_register_success(t_clients *client, t_json_data *json_data) {
    char *new_json = NULL;

    t_json_data json = {.type = JS_REG, .status = 200, .user_id = client->user_id};
    strcpy(json.pers_info.first_name, json_data->pers_info.first_name);
    strcpy(json.pers_info.last_name, json_data->pers_info.last_name);
    strcpy(json.pers_info.login, json_data->pers_info.login);
    strcpy(json.token, client->token);
    new_json = mx_json_make_json(JS_REG, &json);
    mx_logger("JSON write:",  new_json + 4);
    SSL_write(client->ssl, new_json, *(int *)new_json + 4);
    mx_strdel(&new_json);
}
