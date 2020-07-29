#include "uchat_client.h"

char *mx_json_personal_info_upd_request(t_json_data *data) {
    char *s;

    *(int *)s = asprintf(&s, mx_get_json_format_template(JS_PERS_INFO_UPD),
        JS_PERS_INFO_UPD, data->pers_info.login, data->pers_info.password,
        data->pers_info.first_name, data->pers_info.last_name,
        data->user_id, data->token) - 4;
    return s;
}
