#include "uchat_server.h"

char *mx_json_message_delete_in_request(t_json_data *data) {
    char *s;
    int len = asprintf(&s, "0000" MX_REQ_MES_DEL_IN, JS_MES_DEL_IN, data->message.message_id,
            data->message.client1_id);
    *(int *)s = len;

    return s;
}
