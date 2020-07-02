#include "../inc/header.h"

char *mx_json_make_json(enum e_json_types n, t_json_data *data,
                        bool is_request) {
    char *(*fn_p[2 * JS_NUM])(t_json_data *) = {mx_json_log_in_request,
    mx_json_log_in_response, mx_json_log_out_request, mx_json_log_out_response,
    mx_json_message_delete_in_request, mx_json_message_delete_in_response,
    mx_json_message_delete_out_request, mx_json_message_delete_out_response,
    mx_json_message_edit_in_request, mx_json_message_edit_in_response,
    mx_json_message_edit_out_request, mx_json_message_edit_out_response,
    mx_json_message_in_request, mx_json_message_in_response,
    mx_json_message_out_request, mx_json_message_out_response,
    mx_json_register_request, mx_json_register_response};

    return is_request ? fn_p[2 * n](data) : fn_p[2 * n + 1](data);
}
