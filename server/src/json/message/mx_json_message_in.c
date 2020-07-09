#include "uchat_server.h"

char *mx_json_message_in_request(t_json_data *data) {
    char *s;
    int len = asprintf(&s, mx_get_json_format_template(JS_MES_IN), JS_MES_IN, data->message.message_id,
            data->message.client1_id, data->message.client2_id, data->message.text);
    *(int *)s = len;

    return s;
}
