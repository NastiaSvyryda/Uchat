#include "uchat_client.h"

char *mx_json_log_out_request(t_json_data *data) {
    char *s;
    int len = asprintf(&s, "0000" MX_REQ_LOG_OUT, JS_LOG_OUT, data->user_id, data->token);
    *(int *)s = len;

    return s;
}
