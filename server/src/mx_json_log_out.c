#include "../inc/server.h"

char *mx_json_log_out_response(t_json_data *data) {
    char *s;
    int len = asprintf(&s, "0000" MX_RESP_LOG_OUT, JS_LOG_OUT, data->status);
    *(int *)s = len;

    return s;
}
