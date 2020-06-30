#include "../../inc/header.h"

char *mx_json_message_edit_out_request(t_message *data, char *token) {
    char *s;

    asprintf(&s, MX_REQ_MES_EDIT_OUT, JS_MES_EDIT_OUT, data->message_id,
             data->text, data->client1_id, token);
    return s;
}

char *mx_json_message_edit_out_response(t_message *data, int status) {
    char *s;

    asprintf(&s, MX_RESP_MES_EDIT_OUT, JS_MES_EDIT_OUT, status,
             data->message_id);
    return s;
}
