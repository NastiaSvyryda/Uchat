#include "uchat_server.h"

char *mx_json_register_response(t_json_data *data) {
    char *s;

    *(int *)s = asprintf(&s, mx_get_json_format_template(JS_REG), JS_REG, data->status, data->user_id,
        data->pers_info.first_name, data->pers_info.last_name, data->pers_info.login, data->token);
    return s;
}
