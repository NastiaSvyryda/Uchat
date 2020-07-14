#include "uchat_server.h"

static int parse_failed(void) {
    fprintf(stderr, "json parse failed\n");
    return 1;
}

int mx_parse_new_channel(struct json_object *jo, t_json_data *json) {
    struct json_object *buf = json_object_object_get(jo, "new_channel");
    char *buf_str = NULL;
    int uids_arr_size = 0;

    if (!buf || !(buf_str = (char *)json_object_get_string(buf))
        || (strcmp(buf_str, "true") && strcmp(buf_str, "false")))
        return parse_failed();                           
    if (!(json->new_channel = json_object_get_boolean(buf)))
        return 0;
    if (!(buf = json_object_object_get(jo, "channel_name"))
        || !(buf_str = (char *)json_object_get_string(buf))
        || !*buf_str)
        return parse_failed();                           
    strcpy(json->new_channel_data.channel_name, buf_str);
    if (!(buf = json_object_object_get(jo, "user_ids"))
        || !(json->new_channel_data.user_ids_size
            = json_object_array_length(buf)))
        return parse_failed();                           
    json->new_channel_data.user_ids = malloc(sizeof(int)
        * (uids_arr_size = json->new_channel_data.user_ids_size));
    for (int i = 0; i < uids_arr_size; i++)
        json->new_channel_data.user_ids[i]
            = json_object_get_int(json_object_array_get_idx(buf, i));
    return 0;
}
