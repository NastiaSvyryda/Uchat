#include "uchat_client.h"

char *mx_json_log_in_request(t_json_data *data) {
    char *s;

    *(int *)s = asprintf(&s, mx_get_json_format_template(JS_LOG_IN), JS_LOG_IN, data->pers_info.login,
            data->pers_info.password);

    return s;
}
