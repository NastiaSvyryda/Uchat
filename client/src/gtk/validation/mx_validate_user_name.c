#include "uchat_client.h"

_Bool mx_validate_user_name(char *str, void *window) {
    _Bool valid = (str && *str && strlen(str) > 0 && strlen(str) < MX_FIRST_NAME_LEN);
    char *msg;

    asprintf(&msg, "User name must be more than 0 symbols and less than %d symbols!", MX_FIRST_NAME_LEN);
    if (!valid)
        mx_show_popup(window, msg);
    return valid;
}
