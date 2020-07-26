#include "uchat_server.h"

void mx_res_js_register_incorrectly_filled_fields(t_clients *client) {
    t_json_data json = {.type = JS_REG, .status = 412};
    char *new_json = mx_json_make_json(JS_REG, &json);

    mx_logger("JSON write:",  new_json + 4);
    SSL_write(client->ssl, new_json, *(int *)new_json + 4);
    mx_strdel(&new_json);
}
