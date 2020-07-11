#include "uchat_client.h"

char *mx_json_message_out_request(t_json_data *data) {
    char *s;
    int len = asprintf(&s, mx_get_json_format_template(JS_MES_OUT), JS_MES_OUT, data->message.client1_id,
            data->message.channel_id, data->message.text);
    *(int *)s = len;

    return s;
}
