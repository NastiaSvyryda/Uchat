#include "uchat_server.h"

bool mx_valid_login(t_json_data *json) {
    int status = true;

    if ((mx_strlen(json->pers_info.login) == 0)
        || (mx_strlen(json->pers_info.login) > MX_VARCHAR_LEN)
        || (mx_valid_str_isalpha(json->pers_info.login) == false))
        status = false;
    if ((mx_strlen(json->pers_info.password) == 0)
        || (mx_strlen(json->pers_info.password) > MX_VARCHAR_LEN)
        || (mx_valid_str_isalpha(json->pers_info.password) == false))
        status = false;
    return status;
}
