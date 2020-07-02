#include "../inc/client.h"

char *mx_json_message_in_response(t_json_data *data) {
    char *s;
    int len = asprintf(&s, "0000" MX_RESP_MES_IN, JS_MES_IN, data->status, data->message.message_id);
    *(int *)s = len;

    return s;
}
