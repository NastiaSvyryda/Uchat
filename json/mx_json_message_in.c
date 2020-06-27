#include "../inc/header.h"

char *mx_json_message_in_request(t_message *data) {
    char *s;

    asprintf(&s, MX_REQ_MES_IN, "message_in", data->client1_id,
             data->client2_id, data->text, data->message_id);
    return s;
}

char *mx_json_message_in_response(t_message *data, int status) {
    char *s;

    asprintf(&s, MX_RESP_MES_IN, "message_in", status, data->message_id);
    return s;
}
