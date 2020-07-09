#include "uchat_server.h"

static void fill_message_data(t_json_data *json, struct json_object *jo,
                              struct json_object *buf) {
    if (buf)
        json->message.message_id = atoi(json_object_get_string(buf));
    if (json->status == -1) {
        json->message.client1_id = atoi(json_object_get_string(
                json_object_object_get(jo, "client1_id")));
        if (json->type != JS_MES_DEL_OUT && json->type != JS_MES_DEL_IN)
            json->message.text = strdup(json_object_get_string(
                    json_object_object_get(jo, "new_message")));
    }
    else if (json->type == JS_MES_OUT)
        sscanf(json_object_get_string(json_object_object_get(
                jo, "delivery_time")), "%ld", &json->message.delivery_time);
    if ((buf = json_object_object_get(jo, "client2_id")))
        json->message.client2_id = atoi(json_object_get_string(buf));
}

static void fill_personal_data(t_json_data *json, struct json_object *jo) {
    struct json_object *buf;

    if (json->status == -1) {
        if (json->type == JS_LOG_IN || json->type == JS_REG) {
            strcpy(json->pers_info.login, json_object_get_string(
                    json_object_object_get(jo, "login")));
            strcpy(json->pers_info.password, json_object_get_string(
                    json_object_object_get(jo, "password")));
        }
    }
    if ((buf = json_object_object_get(jo, "user_id")))
        json->pers_info.user_id = atoi(json_object_get_string(buf));
    if ((buf = json_object_object_get(jo, "first_name"))) {
        strcpy(json->pers_info.first_name,
               json_object_get_string(buf));
        strcpy(json->pers_info.last_name,
               json_object_get_string(json_object_object_get(
                       jo, "last_name")));
    }
}

t_json_data *mx_json_parse(char *s) {
    t_json_data *json = calloc(1, sizeof(t_json_data));
    struct json_object *jo = json_tokener_parse(s);
    struct json_object *buf = json_object_object_get(jo, "type");

    if (!buf)
        return NULL;
    json->type = atoi(json_object_get_string(buf));
    json->status = (buf = json_object_object_get(jo, "status")) ? atoi(
            json_object_get_string(buf)) : -1;
    if ((buf = json_object_object_get(jo, "security")))
        strcpy(json->token, json_object_get_string(buf));
    if ((buf = json_object_object_get(jo, "message_id"))
        || json->type == JS_MES_OUT)
        fill_message_data(json, jo, buf);
    else
        fill_personal_data(json, jo);
    json_object_put(jo);
    return json;
}
