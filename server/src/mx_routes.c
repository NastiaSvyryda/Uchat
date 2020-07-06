#include "uchat_server.h"

void mx_routes(t_json_data *json, t_clients *client, t_clients *cur_client) {
    if (json->type == JS_REG)
        mx_controller_register();
    if(json->type == JS_LOG_IN)
        mx_controller_login(json, cur_client);
    if (json->type == JS_LOG_OUT)
        mx_controller_log_out();
    if (json->type == JS_MES_OUT)
        mx_controller_message(client, cur_client, json);
}
