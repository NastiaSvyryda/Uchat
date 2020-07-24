#include "uchat_client.h"

char *mx_json_get_users_request(t_json_data *data) {
    char *s;

    *(int *)s = asprintf(&s, mx_get_json_format_template(JS_GET_USERS),
        JS_GET_USERS, data->user_id, data->token);
    return s;
}
