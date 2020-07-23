#include "uchat_server.h"

void mx_res_js_log_out_success(t_clients *cur_client) {
    char *new_json = NULL;

    t_json_data json = {.type = JS_LOG_OUT, .status = 200};
    new_json = mx_json_make_json(JS_LOG_OUT, &json);
    mx_logger("JSON write:",  new_json + 4);
    SSL_write(cur_client->ssl, new_json, mx_strlen(new_json + 4) + 4);
    mx_strdel(&new_json);
}
