#include "uchat_server.h"

static void json_login_unauthorized(t_clients *client) {
    t_json_data json = {.type = JS_LOG_IN, .status = 401};
    char *new_json = mx_json_make_json(JS_LOG_IN, &json);

    mx_logger("JSON write:",  new_json + 4);
    SSL_write(client->ssl, new_json, mx_strlen(new_json + 4) + 4);
    mx_strdel(&new_json);
}

void mx_controller_user_all() {

}