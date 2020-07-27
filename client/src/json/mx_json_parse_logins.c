#include "uchat_client.h"

void mx_parse_logins(struct json_object *u_logins_arr, t_json_data *data) {
    int u_logins_arr_size = json_object_array_length(u_logins_arr);
    struct json_object *id_login_p;

    if (!(data->ids_logins_arr_size = u_logins_arr_size))
        return;
    data->ids_logins_arr = malloc(sizeof(t_id_login) * u_logins_arr_size);
    for (int i = 0; i < u_logins_arr_size; i++) {
        id_login_p = json_object_array_get_idx(u_logins_arr, i);
        data->ids_logins_arr[i].user_id = json_object_get_int(
            json_object_object_get(id_login_p, "user_id"));
        strcpy(data->ids_logins_arr[i].login, json_object_to_json_string(
            json_object_object_get(id_login_p, "login")));
    }
}
