#include "uchat_server.h"

void mx_res_js_login_auth(t_main *main) {
    t_json_data json = {.type = JS_LOG_IN, .status = 403};
    char *new_json = mx_json_make_json(JS_LOG_IN, &json);

    mx_logger("JSON write:",  new_json + 4);
    SSL_write(main->client->ssl, new_json, mx_strlen(new_json + 4) + 4);
    mx_strdel(&new_json);
}
