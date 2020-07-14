#include "uchat_client.h"

void mx_parse_messages(struct json_object *messages_arr, t_json_data *data) {
    int messages_arr_size = json_object_array_length(messages_arr);
    struct json_object *message;

    if (!(data->messages_arr_size = messages_arr_size))
        return;
    data->messages_arr = malloc(sizeof(t_message) * messages_arr_size);
    for (int i = 0; i < messages_arr_size; i++) {
        message = json_object_array_get_idx(messages_arr, i);
        data->messages_arr[i].client1_id = json_object_get_int(
            json_object_object_get(message, "client1_id"));
        data->messages_arr[i].message_id = json_object_get_int(
            json_object_object_get(message, "message_id"));
        data->messages_arr[i].delivery_time = json_object_get_int(
            json_object_object_get(message, "delivery_time"));
        strcpy(data->messages_arr[i].text, json_object_to_json_string(
            json_object_object_get(message, "text")));
    }
}
