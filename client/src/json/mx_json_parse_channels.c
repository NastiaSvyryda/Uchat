#include "uchat_client.h"

static void parse_user_ids_arr(struct json_object *uids_arr,
                               t_channel *channel) {
    int uids_arr_size = json_object_array_length(uids_arr);

    if (!(channel->user_ids_size = uids_arr_size))
        return;
    channel->user_ids = malloc(sizeof(int) * uids_arr_size);
    for (int i = 0; i < uids_arr_size; i++)
        channel->user_ids[i] = json_object_get_int(json_object_array_get_idx(
            uids_arr, i));
}

void mx_parse_channels(struct json_object *ch_arr, t_json_data *data) {
    int ch_arr_size = json_object_array_length(ch_arr);
    struct json_object *uids_arr;
    struct json_object *channel_p;

    if (!(data->channels_arr_size = ch_arr_size))
        return;
    data->channels_arr = malloc(sizeof(t_channel) * ch_arr_size);
    for (int i = 0; i < ch_arr_size; i++) {
        channel_p = json_object_array_get_idx(ch_arr, i);
        data->channels_arr[i].channel_id = json_object_get_int(
            json_object_object_get(channel_p, "channel_id"));
        strcpy(data->channels_arr[i].channel_name, json_object_to_json_string(
            json_object_object_get(channel_p, "channel_name")));
        data->channels_arr[i].last_mes_time = json_object_get_int(
            json_object_object_get(channel_p, "last_mes_time"));
        if (!json_object_array_length((uids_arr = 
            json_object_object_get(channel_p, "user_ids"))))
            continue;
        parse_user_ids_arr(uids_arr, &data->channels_arr[i]);
    }
}
