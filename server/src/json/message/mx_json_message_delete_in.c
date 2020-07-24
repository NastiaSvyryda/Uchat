#include "uchat_server.h"

char *mx_json_message_delete_in_request(t_json_data *data) {
    char *s;

    *(int *)s = asprintf(&s, mx_get_json_format_template(JS_MES_DEL_IN), JS_MES_DEL_IN, data->message.message_id,
            data->message.channel_id);
    return s;
}
