#include "uchat_client.h"

char *mx_json_register_request(t_json_data *data) {
    char *s;

    *(int *)s = asprintf(&s, mx_get_json_format_template(JS_REG), JS_REG, data->pers_info.login, data->pers_info.password,
            data->pers_info.first_name, data->pers_info.last_name);

    return s;
}
