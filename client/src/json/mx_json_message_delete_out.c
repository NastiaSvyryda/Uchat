#include "uchat_client.h"

char *mx_json_message_delete_out_request(t_json_data *data) {
    char *s;
    int len = asprintf(&s, mx_get_json_format_template(JS_MES_DEL_OUT), JS_MES_DEL_OUT, data->message.message_id,
            data->message.client1_id, data->token);
    *(int *)s = len;

    return s;
}
