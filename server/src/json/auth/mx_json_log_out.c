#include "uchat_server.h"

char *mx_json_log_out_response(t_json_data *data) {
    char *s;
    int len = asprintf(&s, mx_get_json_format_template(JS_LOG_OUT), JS_LOG_OUT, data->status);
    *(int *)s = len;

    return s;
}
