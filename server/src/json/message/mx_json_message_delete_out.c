#include "uchat_server.h"

char *mx_json_message_delete_out_response(t_json_data *data) {
    char *s;
    int len = asprintf(&s, mx_get_json_format_template(JS_MES_DEL_OUT), JS_MES_DEL_OUT, data->status,
            data->message.message_id);
    *(int *)s = len;

    return s;
}
