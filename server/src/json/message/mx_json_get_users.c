#include "uchat_server.h"

static struct json_object *user_logins(t_id_login *id_login) {
    char *s;
    struct json_object *jo;

    asprintf(&s, mx_get_json_format_supply_template(JS_GET_USERS),
             id_login->user_id, id_login->login);
    jo = json_tokener_parse(s);
    free(s);
    return jo;
}

char *mx_json_get_users_response(t_json_data *data) {
    char *tmp;
    char *s;
    int len = 0;
    struct json_object *jo;
    struct json_object *arr;

    asprintf(&tmp, mx_get_json_format_template(JS_GET_USERS), JS_GET_USERS,
        data->status);
    jo = json_tokener_parse(tmp);
    free(tmp);
    arr = json_object_object_get(jo, "user_logins");
    for (int i = 0; i < data->ids_logins_arr_size; i++)
        json_object_array_add(arr, user_logins(&data->ids_logins_arr[i]));
    s = malloc((len = strlen((tmp = (char *)json_object_to_json_string_ext(
        jo, MX_JSON_TO_STR_FLAGS)))) + 5);
    *(int *)s = len;
    strcpy(s + 4, tmp);
    json_object_put(jo);
    return s;
}
