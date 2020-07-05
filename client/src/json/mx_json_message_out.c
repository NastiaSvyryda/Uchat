#include "uchat_client.h"

char *mx_json_message_out_request(t_json_data *data) {
    char *s;
    int len = asprintf(&s, "0000" MX_REQ_MES_OUT, JS_MES_OUT, data->message.client1_id,
            data->message.client2_id, data->message.text);
    *(int *)s = len;

    return s;
}
