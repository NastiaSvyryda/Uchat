#include "uchat_server.h"

static char *file_to_str(const char *filename) {
    char buf[2100] = "    ";
    int fd = open(filename, O_RDONLY);

    if (fd == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        errno = 0;
        return NULL;
    }
    read(fd, buf + 4, 2048);
    close(fd);
    return strdup(buf);
}

char *mx_get_json_format_template(enum e_json_types type) {
    static char *templ_arr[JS_NUM];
    char name_buf[128];

    if (!*templ_arr) {
        for (int i = 0; i < JS_NUM; i++) {
            sprintf(name_buf, MX_JSON_TEMPLATE, i);
            templ_arr[i] = file_to_str(name_buf);
        }
    }
    return templ_arr[type];
}

char *mx_json_make_json(enum e_json_types type, t_json_data *data) {
    char *(*fn_p[JS_NUM])(t_json_data *)
    = { mx_json_register_response,
        mx_json_log_in_response,
        mx_json_log_out_response,
        mx_json_message_delete_in_request,
        mx_json_message_delete_out_response,
        mx_json_message_edit_in_request,
        mx_json_message_edit_out_response,
        mx_json_message_in_request,
        mx_json_message_out_response };

    return fn_p[type](data);
}
