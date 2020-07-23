#include "uchat_server.h"

void mx_routes(t_json_data *json, t_main *main, t_clients *cur_client) {
    main->client = cur_client;
    if (json->type == JS_REG)
        mx_controller_register(json, main);
    else if (json->type == JS_LOG_IN)
        mx_controller_login(json, main);
    else if(mx_valid_token(json->user_id, json->token) == true) {
        if (json->type == JS_LOG_OUT)
            mx_controller_log_out(json, main, cur_client);
        else if (json->type == JS_MES_OUT)
            mx_controller_message(main, json);
        else if (json->type == JS_MES_EDIT_OUT)
            mx_controller_edit_message(json, main);
        else if (json->type == JS_MES_HIST)
            mx_controller_message_history(json, cur_client);
        else if (json->type == JS_GET_USERS)
            mx_controller_user_all(cur_client);
        else if (json->type == JS_MES_DEL_OUT)
            mx_controller_delete_message(json, main);
    }
}
