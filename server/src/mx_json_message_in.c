#include "../inc/server.h"

char *mx_json_message_in_request(t_json_data *data) {
    char *s;
    int len = asprintf(&s, "0000" MX_REQ_MES_IN, JS_MES_IN, data->message.message_id,
             data->message.client1_id, data->message.client2_id, data->message.text);
    *(int *)s = len;

    return s;
}
