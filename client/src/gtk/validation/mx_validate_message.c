#include "uchat_client.h"

_Bool mx_validate_message(char *str, void *window) {
    _Bool valid = (str && *str && strlen(str) > 0 && strlen(str) < MX_MAX_MESSAGE_LEN);
    char *msg;

    asprintf(&msg, "Message must be more than 0 symbols and less than %d symbols!", MX_MAX_MESSAGE_LEN);
    if (!valid)
        mx_show_popup(window, msg);
    return valid;
}
