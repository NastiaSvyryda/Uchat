#include "uchat_server.h"

void mx_res_js_user_edit_success(t_clients *client) {
    char *new_json = NULL;
    t_json_data json = {.type = JS_PERS_INFO_UPD, .status = 200};

    new_json = mx_json_make_json(JS_PERS_INFO_UPD, &json);
    mx_logger("JSON write:",  new_json + 4);
    SSL_write(client->ssl, new_json, *(int *)new_json + 4);
    mx_strdel(&new_json);
}
