#include "uchat_server.h"

static bool is_valid_json_data(t_json_data *data) {
    if ((data->type == JS_REG && *data->pers_info.login
            && *data->pers_info.password && *data->pers_info.first_name
            && *data->pers_info.last_name)
        || (data->type == JS_LOG_IN && *data->pers_info.login
            && *data->pers_info.password)
        || (data->type == JS_LOG_OUT && data->pers_info.user_id && *data->token)
        || (data->type == JS_MES_DEL_IN && data->message.channel_id
            && data->status && data->message.message_id && *data->token)
        || (data->type == JS_MES_DEL_OUT && data->message.message_id
            && data->message.client1_id && *data->token)
        || (data->type == JS_MES_EDIT_IN && data->status
            && data->message.message_id && data->message.channel_id
            && *data->token)
        || (data->type == JS_MES_EDIT_OUT && data->message.message_id
            && *data->message.text && data->message.client1_id && *data->token)
        || (data->type == JS_MES_IN && data->status && data->message.message_id)
        || (data->type == JS_MES_OUT && data->message.client1_id
            && data->message.channel_id && *data->message.text))
        return true;
    return false;
}

static void *parse_failed(struct json_object *jo, t_json_data *data,
                          char *message) {
    fprintf(stderr, "json parse error: invalid %s\n", message);
    json_object_put(jo);
    if (data && data->message.text)
        free(data->message.text);
    free(data);
    errno = 0;
    return NULL;
}

static int fill_message_data(t_json_data *json, struct json_object *jo,
                              struct json_object *buf) {
    if (buf && ((json->message.message_id = json_object_get_int(buf)) == 0
            && errno == EINVAL))
        return 1;
    if ((buf = json_object_object_get(jo, "client1_id")))
        if ((json->message.client1_id = json_object_get_int(buf)) == 0
            && errno == EINVAL)
            return 1;
    if ((buf = json_object_object_get(jo, "channel_id")))
        if ((json->message.channel_id = json_object_get_int(buf)) == 0
            && errno == EINVAL)
            return 1;
    if ((buf = json_object_object_get(jo, "new_message")))
        json->message.text = strdup(json_object_get_string(buf));
    return 0;
}

static int fill_personal_data(t_json_data *json, struct json_object *jo) {
    struct json_object *buf;

    if ((buf = json_object_object_get(jo, "login")))
        strcpy(json->pers_info.login,
               json_object_get_string(buf));
    if ((buf = json_object_object_get(jo, "password")))
        strcpy(json->pers_info.password,
               json_object_get_string(buf));
    if ((buf = json_object_object_get(jo, "first_name")))
        strcpy(json->pers_info.first_name,
               json_object_get_string(buf));
    if ((buf = json_object_object_get(jo, "last_name")))
        strcpy(json->pers_info.last_name,
               json_object_get_string(buf));
    if ((buf = json_object_object_get(jo, "user_id")))
        if ((json->pers_info.user_id = json_object_get_int(buf)) == 0
            && errno == EINVAL)
            return 1;
    return 0;
}

t_json_data *mx_json_parse(char *s) {
    t_json_data *json = calloc(1, sizeof(t_json_data));
    struct json_object *jo = json_tokener_parse(s);
    struct json_object *buf = json_object_object_get(jo, "type");

    if (!jo || !buf)
        return parse_failed(jo, json, "wrong json");
    if ((json->type = json_object_get_int(buf)) == 0 && errno == EINVAL)
        return parse_failed(jo, json, "incorrect format of TYPE field");
    if ((json->status = (buf = json_object_object_get(jo, "status")) ?
        json_object_get_int(buf) : -1) == 0 && errno == EINVAL)
        return parse_failed(jo, json, "incorrect format of STATUS field");
    if ((buf = json_object_object_get(jo, "token")))
        strcpy(json->token, json_object_get_string(buf));
    if ((buf = json_object_object_get(jo, "message_id"))
        || json->type == JS_MES_OUT) {
        if (fill_message_data(json, jo, buf))
            return parse_failed(jo, json, "fill_message_data fail");
    }
    else if (fill_personal_data(json, jo))
        return parse_failed(jo, json, "fill_personal_data fail");
    // printf("BEFORE IS_VALID\n");
    if (!is_valid_json_data(json))
        return parse_failed(jo, json, "missing fields in json");
    json_object_put(jo);
    return json;
}
