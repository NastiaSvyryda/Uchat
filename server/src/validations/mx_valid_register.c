#include "uchat_server.h"

int mx_valid_register(t_json_data *json) {
    int status = true;

    if ((mx_strlen(json->pers_info.login) > MX_VARCHAR_LEN)
        || (mx_strlen(json->pers_info.login) == 0)
        || (mx_valid_str_isalpha(json->pers_info.login) == false))
        status = false;
    if ((mx_strlen(json->pers_info.password) > MX_VARCHAR_LEN)
        || (mx_strlen(json->pers_info.password) == 0)
        || (mx_valid_str_isalpha(json->pers_info.login) == false))
        status = false;
    if ((mx_strlen(json->pers_info.first_name) > MX_MAX_NAME_LEN)
        || (mx_strlen(json->pers_info.first_name) == 0)
        || (mx_valid_str_isalpha(json->pers_info.login) == false))
        status = false;
    if ((mx_strlen(json->pers_info.last_name) > MX_MAX_NAME_LEN)
        || (mx_strlen(json->pers_info.first_name) == 0)
        ||  (mx_valid_str_isalpha(json->pers_info.login) == false))
        status = false;
    return status;
}
