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

static int fill_message_data(t_json_data *json, struct json_object *jo,
                              struct json_object *buf) {
    if (buf && ((json->message.message_id = json_object_get_int(buf)) == 0
            && errno == EINVAL))
        return 1;
    if (json->status == -1) {
        if ((json->message.client1_id = json_object_get_int(
            json_object_object_get(jo, "client1_id"))) == 0 && errno == EINVAL)
            return 1;
        if ((buf = json_object_object_get(jo, "new_message")))
            json->message.text = strdup(json_object_get_string(buf));
        if ((buf = json_object_object_get(jo, "client2_id")))
            if ((json->message.client2_id = json_object_get_int(buf)) == 0
                && errno == EINVAL)
                return 1;
    }
    else if ((buf = json_object_object_get(jo, "delivery_time")))
        if ((json->message.delivery_time = json_object_get_int(buf)) == 0
            && errno == EINVAL)
            return 1;
    return 0;
}

static int fill_personal_data(t_json_data *json, struct json_object *jo) {
    struct json_object *buf;

    if ((buf = json_object_object_get(jo, "user_id")))
        if ((json->pers_info.user_id = json_object_get_int(buf)) == 0
            && errno == EINVAL)
            return 1;
    if ((buf = json_object_object_get(jo, "first_name"))) {
        strcpy(json->pers_info.first_name,
               json_object_get_string(buf));
        strcpy(json->pers_info.last_name,
               json_object_get_string(json_object_object_get(
                       jo, "last_name")));
    }
    return 0;
}

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
    if ((buf = json_object_object_get(jo, "message_id"))) {
        if (fill_message_data(json, jo, buf))
            return parse_failed(jo, json);
    }
    else if (fill_personal_data(json, jo))
        return parse_failed(jo, json);
    json_object_put(jo);
    return json;
}
