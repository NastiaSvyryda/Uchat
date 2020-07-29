#include "uchat_client.h"

_Bool mx_validate_login(char *str, void *window) {
    _Bool valid = (str && *str && strlen(str) > 0 && strlen(str) < MX_LOGIN_LEN);
    char *msg;

    for (int i = 0; str[i]; i++)
        if (str[i] == ' ')
            valid = 0;
    asprintf(&msg, "Login cannot contain space and must be more than 0 symbols and less than %d symbols!", MX_LOGIN_LEN);
    if (!valid)
        mx_show_popup(window, msg);
    return valid;
}
