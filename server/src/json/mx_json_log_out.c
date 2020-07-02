#include "uchat_server.h"

char *mx_json_log_out_response(int status) {
    char *s;

    asprintf(&s, MX_RESP_LOG_OUT, JS_LOG_OUT, status);
    return s;
}
