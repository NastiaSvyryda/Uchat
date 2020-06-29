#include "../inc/header.h"

char *mx_json_message_edit_in_request(t_message *data) {
    char *s;

    asprintf(&s, MX_REQ_MES_EDIT_IN, JS_MES_EDIT_IN, data->message_id,
             data->text, data->client1_id);
    return s;
}

char *mx_json_message_edit_in_response(t_message *data, int status,
                                       char *token) {
    char *s;

    asprintf(&s, MX_RESP_MES_EDIT_IN, JS_MES_EDIT_IN, status,
             data->message_id, data->client2_id, token);
    return s;
}
