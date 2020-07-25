#include "uchat_client.h"

char *mx_json_log_out_request(t_json_data *data) {
    char *s;

    *(int *)s = asprintf(&s, mx_get_json_format_template(JS_LOG_OUT),
        JS_LOG_OUT, data->user_id, data->token);
    return s;
}
