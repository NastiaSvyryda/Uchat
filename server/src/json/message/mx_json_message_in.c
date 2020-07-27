#include "uchat_server.h"

char *mx_json_message_in_request(t_json_data *data) {
    char *tmp;
    char *s;
    int len = 0;
    struct json_object *jo;
    struct json_object *arr;

    asprintf(&tmp, mx_get_json_format_template(JS_MES_IN), JS_MES_IN,
        data->message.message_id, data->message.client1_id,
        data->message.channel_id, data->message.text, data->pers_info.login,
        data->new_channel ? "true" : "false",
        data->new_channel ? data->new_channel_data.channel_name : "");
    jo = json_tokener_parse(tmp);
    free(tmp);
    if (data->new_channel) {
        arr = json_object_object_get(jo, "user_ids");
        for (int i = 0; i < data->new_channel_data.user_ids_size; i++)
            json_object_array_add(arr, json_object_new_int(
                data->new_channel_data.user_ids[i]));
    }
    s = malloc((len = strlen((tmp = (char *)json_object_to_json_string_ext(
        jo, MX_JSON_TO_STR_FLAGS)))) + 5);
    *(int *)s = len;
    strcpy(s + 4, tmp);
    json_object_put(jo);
    return s;
}
