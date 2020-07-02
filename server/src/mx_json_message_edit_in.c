#include "../inc/server.h"
char *mx_json_message_edit_in_request(t_json_data *data) {
    char *s;
    int len = asprintf(&s, "0000" MX_REQ_MES_EDIT_IN, JS_MES_EDIT_IN, data->message.message_id,
             data->message.text, data->message.client1_id);
    *(int *)s = len;

    return s;
}
