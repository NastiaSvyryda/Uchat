#include "uchat_client.h"

static void *parse_failed(struct json_object *jo, t_json_data *json) {
    json_object_put(jo);
    if (json && json->message.text)
        free(json->message.text);
    free(json);
    errno = 0;
    fprintf(stderr, "json parse failed\n");
    return NULL;
}

// static int fill_message_data(t_json_data *json, struct json_object *jo,
//                              struct json_object *buf) {
// }

// static int fill_personal_data(t_json_data *json, struct json_object *jo) {
//     struct json_object *buf;

// }

t_json_data *mx_json_parse(char *s) {
    t_json_data *json = calloc(1, sizeof(t_json_data));
    struct json_object *jo = json_tokener_parse(s);
    struct json_object *buf = json_object_object_get(jo, "type");

    if (!jo || !buf)
        return parse_failed(jo, json);
    if ((json->type = json_object_get_int(buf)) == 0 && errno == EINVAL)
        return parse_failed(jo, json);
    if ((json->status = (buf = json_object_object_get(jo, "status")) ?
        json_object_get_int(buf) : -1) == 0 && errno == EINVAL)
        return parse_failed(jo, json);
    if ((buf = json_object_object_get(jo, "token")))
        strcpy(json->token, json_object_get_string(buf));
    if ((buf = json_object_object_get(jo, "user_id")))
        if ((json->user_id = json_object_get_int(buf)) == 0
            && errno == EINVAL)
            return parse_failed(jo, json);
    if ((buf = json_object_object_get(jo, "first_name"))) {
        strcpy(json->pers_info.first_name,
               json_object_get_string(buf));
        strcpy(json->pers_info.last_name,
               json_object_get_string(json_object_object_get(
                       jo, "last_name")));
    }

    if ((buf = json_object_object_get(jo, "message_id"))
        && ((json->message.message_id = json_object_get_int(buf)) == 0
        && errno == EINVAL))
        return parse_failed(jo, json);
    if ((json->message.client1_id = json_object_get_int(
        json_object_object_get(jo, "client1_id"))) == 0 && errno == EINVAL)
        return parse_failed(jo, json);
    if ((buf = json_object_object_get(jo, "new_message")))
        json->message.text = strdup(json_object_get_string(buf));
    if ((buf = json_object_object_get(jo, "channel_id")))
        if ((json->message.channel_id = json_object_get_int(buf)) == 0
            && errno == EINVAL)
            return parse_failed(jo, json);
    if ((buf = json_object_object_get(jo, "delivery_time")))
        if ((json->message.delivery_time = json_object_get_int(buf)) == 0
            && errno == EINVAL)
            return parse_failed(jo, json);
    if (json->type == JS_LOG_IN)
        mx_parse_channels(json_object_object_get(jo, "channels"), json);
    else if (json->type == JS_MES_IN && mx_parse_new_channel(jo, json))
        return parse_failed(jo, json);
    else if (json->type == JS_GET_USERS)
        mx_parse_logins(json_object_object_get(jo, "user_logins"), json);
    else if (json->type == JS_MES_HIST)
        mx_parse_messages(json_object_object_get(jo, "messages"), json);
    json_object_put(jo);
    return json;
}
