#include "uchat_client.h"

_Bool mx_validate_password(char *str, void *window) {
    _Bool valid = (str && *str && strlen(str) > 7 && strlen(str) < MX_MAX_PASSWORD_LEN);
    char *msg;

    asprintf(&msg, "Password must be more than 7 symbols and less than %d symbols!", MX_MAX_PASSWORD_LEN);
    if (!valid)
        mx_show_popup(window, msg);
    return valid;
}
