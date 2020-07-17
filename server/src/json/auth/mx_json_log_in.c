#include "uchat_server.h"

static struct json_object *user_ids(t_channel *channel) {
    char *s;
    struct json_object *jo;
    struct json_object *arr;

    asprintf(&s, mx_get_json_format_supply_template(JS_LOG_IN),
             channel->channel_id, channel->channel_name,
             channel->last_mes_time);
    jo = json_tokener_parse(s);
    arr = json_object_object_get(jo, "user_ids");
    for (int i = 0; i < channel->user_ids_size; i++)
        json_object_array_add(arr, json_object_new_int(channel->user_ids[i]));
    free(s);
    return jo;
}

char *mx_json_log_in_response(t_json_data *data) {
    char *tmp;
    char *s;
    int len = 0;
    struct json_object *jo;
    struct json_object *arr;

    asprintf(&tmp, mx_get_json_format_template(JS_LOG_IN), JS_LOG_IN,
        data->status, data->user_id, data->pers_info.first_name,
        data->pers_info.last_name, data->pers_info.login, data->token, data->channels_arr_size);
    jo = json_tokener_parse(tmp);
    free(tmp);
    arr = json_object_object_get(jo, "channels");
    for (int i = 0; i < data->channels_arr_size; i++)
        json_object_array_add(arr, user_ids(&data->channels_arr[i]));
    s = malloc((len = strlen((tmp = (char *)json_object_to_json_string_ext(
        jo, MX_JSON_TO_STR_FLAGS)))) + 5);
    *(int *)s = len;
    strcpy(s + 4, tmp);
    json_object_put(jo);
    return s;
}
