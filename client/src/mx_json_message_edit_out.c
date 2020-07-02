#include "../inc/client.h"

char *mx_json_message_edit_out_request(t_json_data *data) {
    char *s;
    int len = asprintf(&s, "0000" MX_REQ_MES_EDIT_OUT, JS_MES_EDIT_OUT, data->message.message_id,
             data->message.text, data->message.client1_id, data->token);
    *(int *)s = len;

    return s;
}
