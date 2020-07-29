#include "uchat_client.h"

gboolean mx_receiver(__attribute__((unused)) GIOChannel *chan,
                    __attribute__((unused)) GIOCondition condition, void *data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    char *json_str = NULL;
    int length = 0;
    t_json_data *json = NULL;
    SSL_read(mwo->ssl, &length, 4);
    if ( length > 0) {
        json_str = mx_strnew(length);
        mx_SSL_read_to_str(mwo->ssl, json_str, length);
        json = mx_json_parse(json_str);
        if (json != NULL) {
            if (json->type == JS_REG)
                mx_receiver_reg(mwo, json);
            else if (json->type == JS_LOG_IN)
                mx_receiver_login(mwo, json);
            else if (json->type == JS_MES_IN) {
                mx_printchar('\a');
                mx_add_out_message(mwo, json);
            }
            else if (json->type == JS_MES_OUT)
                mx_fill_message_info_out(mwo, json);
            else if (json->type == JS_MES_DEL_IN)
                mx_receiver_mes_del(mwo, json);
            else if (json->type == JS_MES_EDIT_IN)
                mx_receiver_mes_edit(mwo, json);
            else if (json->type == JS_GET_USERS)
                mx_receiver_get_users(mwo, json);
            else if (json->type == JS_MES_HIST)
                mx_receiver_get_history(mwo, json);
            if (json->message.text != NULL)
                mx_strdel(&json->message.text);
        }
    }
    return TRUE;
}
