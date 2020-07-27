#include "uchat_server.h"

bool mx_valid_login_auth(t_list *list) {
    int status = true;

    if (mx_strcmp(list->next->next->next->next->next->data, "NULL") == 0)
        status = false;
    return status;
}
