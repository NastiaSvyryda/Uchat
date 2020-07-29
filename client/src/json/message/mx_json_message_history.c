#include "uchat_client.h"

char *mx_json_message_history_request(t_json_data *data) {
    char *s;

    *(int *)s = asprintf(&s, mx_get_json_format_template(JS_MES_HIST),
        JS_MES_HIST, data->message.channel_id, data->message.last_message_id,
        data->user_id, data->token) - 4;
    return s;
}
