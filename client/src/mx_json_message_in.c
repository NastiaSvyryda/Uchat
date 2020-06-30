#include "../../inc/header.h"

char *mx_json_message_in_request(t_message *data) {
    char *s;

    asprintf(&s, MX_REQ_MES_IN, JS_MES_IN, data->message_id,
             data->client1_id, data->client2_id, data->text);
    return s;
}

char *mx_json_message_in_response(t_message *data, int status) {
    char *s;

    asprintf(&s, MX_RESP_MES_IN, JS_MES_IN, status, data->message_id);
    return s;
}
