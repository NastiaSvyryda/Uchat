#include "uchat_client.h"

char *mx_json_message_delete_in_response(t_json_data *data) {
    char *s;
    int len = asprintf(&s, "0000" MX_RESP_MES_DEL_IN, JS_MES_DEL_IN, data->status,
            data->message.message_id, data->message.client2_id, data->token);
    *(int *)s = len;

    return s;
}
