#include "uchat_server.h"

static bool is_valid_json_data(t_json_data *data) {
    if ((data->type == JS_REG && *data->pers_info.login
            && *data->pers_info.password && *data->pers_info.first_name
            && *data->pers_info.last_name)
        || (data->type == JS_LOG_IN && *data->pers_info.login
            && *data->pers_info.password)
        || (data->type == JS_LOG_OUT)
        || (data->type == JS_MES_DEL_IN && data->message.channel_id
            && data->status && data->message.message_id)
        || (data->type == JS_MES_DEL_OUT && data->message.message_id
            && data->message.channel_id && data->user_id)
        || (data->type == JS_MES_EDIT_IN && data->status
            && data->message.message_id && data->message.channel_id)
        || (data->type == JS_MES_EDIT_OUT && data->message.message_id
            && *data->message.text && data->message.client1_id)
        || (data->type == JS_MES_IN && data->status && data->message.message_id)
        || (data->type == JS_MES_OUT && data->message.client1_id
            && data->message.channel_id && *data->message.text)
        || (data->type == JS_GET_USERS)
        || (data->type == JS_MES_HIST)
        || (data->type == JS_PERS_INFO_UPD))
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

// static int fill_message_data(t_json_data *json, struct json_object *jo,
//                               struct json_object *buf) {
// }

// static void fill_personal_data(t_json_data *json, struct json_object *jo) {
//     struct json_object *buf;

// }

t_json_data *mx_json_parse(char *s) {
    t_json_data *json = calloc(1, sizeof(t_json_data));
    struct json_object *jo = json_tokener_parse(s);
    struct json_object *buf = json_object_object_get(jo, "type");

    if (!jo || !buf)
        return parse_failed(jo, json, "str: json_tokener_parse error");
    if ((json->type = json_object_get_int(buf)) == 0 && errno == EINVAL)
        return parse_failed(jo, json, "format of TYPE field");
    if (json->type != JS_LOG_IN && json->type != JS_REG
        && (!(buf = json_object_object_get(jo, "user_id"))
        || (json->user_id = json_object_get_int(buf)) == 0))
        return parse_failed(jo, json, "or missing user_id field");
    if (json->type != JS_LOG_IN && json->type != JS_REG
        && (!(buf = json_object_object_get(jo, "token"))
        || !*json_object_get_string(buf)))
        return parse_failed(jo, json, "or missing TOKEN field");
    else
        strcpy(json->token, json_object_get_string(buf));
    if ((json->status = (buf = json_object_object_get(jo, "status")) ?
        json_object_get_int(buf) : -1) == 0 && errno == EINVAL)
        return parse_failed(jo, json, "format of STATUS field");
    if ((buf = json_object_object_get(jo, "message_id"))
        && ((json->message.message_id = json_object_get_int(buf)) == 0
        && errno == EINVAL))
        return parse_failed(jo, json, "message_id field");
    if ((buf = json_object_object_get(jo, "client1_id")))
        if ((json->message.client1_id = json_object_get_int(buf)) == 0
            && errno == EINVAL)
            return parse_failed(jo, json, "client1_id field");
    if ((buf = json_object_object_get(jo, "channel_id")))
        if ((json->message.channel_id = json_object_get_int(buf)) == 0
            && errno == EINVAL)
            return parse_failed(jo, json, "channel_id field");
    if ((buf = json_object_object_get(jo, "new_message")))
        json->message.text = strdup(json_object_get_string(buf));
    if ((buf = json_object_object_get(jo, "last_message_id"))
        && ((json->message.last_message_id = json_object_get_int(buf)) == 0
        && errno == EINVAL))
        return parse_failed(jo, json, "last_message_id field");
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
    if (json->type == JS_MES_OUT && mx_parse_new_channel(jo, json))
        return parse_failed(jo, json, "or missing fields");
    if (!is_valid_json_data(json))
        return parse_failed(jo, json, "JSON format: missing fields");
    json_object_put(jo);
    return json;
}
