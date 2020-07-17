#include "uchat_server.h"

static struct json_object *message(t_message *message) {
    char *s;
    struct json_object *jo;

    asprintf(&s, mx_get_json_format_supply_template(JS_MES_HIST),
             message->client1_id, message->message_id,
             message->delivery_time,
             message->text);
    jo = json_tokener_parse(s);
    free(s);
    return jo;
}

char *mx_json_message_history_response(t_json_data *data) {
    char *tmp;
    char *s;
    int len = 0;
    struct json_object *jo;
    struct json_object *arr;

    asprintf(&tmp, mx_get_json_format_template(JS_MES_HIST), JS_MES_HIST,
        data->status, data->message.channel_id);
    jo = json_tokener_parse(tmp);
    free(tmp);
    arr = json_object_object_get(jo, "messages");
    for (int i = 0; i < data->messages_arr_size; i++)
        json_object_array_add(arr, message(&data->messages_arr[i]));
    s = malloc((len = strlen((tmp = (char *)json_object_to_json_string_ext(
        jo, MX_JSON_TO_STR_FLAGS)))) + 5);
    *(int *)s = len;
    strcpy(s + 4, tmp);
    json_object_put(jo);
    return s;
}
