#include "../inc/server.h"

char *mx_json_message_edit_out_response(t_json_data *data) {
    char *s;
    int len = asprintf(&s, "0000" MX_RESP_MES_EDIT_OUT, JS_MES_EDIT_OUT, data->status,
             data->message.message_id);
    *(int *)s = len;

    return s;
}
