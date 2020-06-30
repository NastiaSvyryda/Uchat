#include "../inc/header.h"

char *mx_json_message_out_request(t_message *data) {
    char *s;

    asprintf(&s, MX_REQ_MES_OUT, "message_out", data->client1_id,
             data->client2_id, data->text);
    return s;
}

char *mx_json_message_out_response(t_message *data, int status) {
    char *s;

    asprintf(&s, MX_RESP_MES_OUT, "message_out", status, data->deliver_time,
             data->message_id);
    return s;
}
