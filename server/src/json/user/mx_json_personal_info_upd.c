#include "uchat_server.h"

char *mx_json_personal_info_upd_response(t_json_data *data) {
    char *s;

    *(int *)s = asprintf(&s, mx_get_json_format_template(JS_PERS_INFO_UPD),
        JS_PERS_INFO_UPD, data->status);
    return s;
}
