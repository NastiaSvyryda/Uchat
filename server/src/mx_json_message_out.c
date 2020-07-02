#include "../inc/server.h"

char *mx_json_message_out_response(t_json_data *data) {
    char *s;
    int len = asprintf(&s, "0000" MX_RESP_MES_OUT, JS_MES_OUT, data->status, data->message.message_id,
             data->message.delivery_time);
    *(int *)s = len;

    return s;
}
