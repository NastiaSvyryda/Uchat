#include "../../inc/header.h"

char *mx_json_log_out_request(int user_id, char *token) {
    char *s;

    asprintf(&s, MX_REQ_LOG_OUT, JS_LOG_OUT, user_id, token);
    return s;
}

char *mx_json_log_out_response(int status) {
    char *s;

    asprintf(&s, MX_RESP_LOG_OUT, JS_LOG_OUT, status);
    return s;
}
