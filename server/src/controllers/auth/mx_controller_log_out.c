#include "uchat_server.h"

static void json_log_out_success(t_clients *client) {
    char *new_json = NULL;

    t_json_data json = {.type = JS_LOG_OUT, .status = 200};
    new_json = mx_json_make_json(JS_LOG_OUT, &json);
    mx_logger("JSON write:",  new_json + 4);
    SSL_write(client->ssl, new_json, mx_strlen(new_json + 4) + 4);
    mx_strdel(&new_json);
}

void mx_controller_log_out(t_json_data *json, t_clients *client) {
    char *set = NULL;
    char *where = NULL;
    char **fill = mx_model_user_fill_table();

    asprintf(&set, "%s = %s",
             fill[5],
             "NULL");
    asprintf(&where, "%s = %i",
             fill[0],
             json->user_id);
    mx_update_database(mx_model_user_database(), mx_model_user_name_table(), set, where);
    mx_free_client(&client);
    json_log_out_success(client);
    mx_strdel(&set);
    mx_strdel(&where);
    mx_del_strarr(&fill);
}
