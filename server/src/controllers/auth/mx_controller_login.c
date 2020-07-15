#include "uchat_server.h"

static void mx_get_channel_name_from_database_channels(t_json_data *json, int index) {
    char *where = NULL;
    char **fill = NULL;
    t_list *data = NULL;

    fill = mx_model_channel_fill_table();
    asprintf(&where, "%s=%d",
             fill[0],
             json->channels_arr[index].channel_id);
    data = mx_read_database(mx_model_channel_database(), mx_model_channel_name_table(), "name", where);
    strcpy(json->channels_arr[index].channel_name, data->data);
    mx_strdel(&where);
    mx_del_strarr(&fill);
}

static void mx_fill_channels(t_json_data *json) {
    char *where = NULL;
    char **fill = NULL;
    t_list *channel_id = NULL;
    t_list *user_id = NULL;
    int k = 0;
    int j = 0;
    fill = mx_model_user_channel_fill_table();
    asprintf(&where ,"%s=%d",
             fill[1],
             json->user_id);
    channel_id = mx_read_database(mx_model_user_channel_database(), mx_model_user_channel_name_table(), "channel_id", where);
    json->channels_arr_size = mx_list_size(channel_id);
    json->channels_arr = malloc(sizeof(t_channel) * json->channels_arr_size);
    while (channel_id != NULL) {
         json->channels_arr[k].channel_id = mx_atoi(channel_id->data);
         user_id = mx_get_user_id_from_database_channels(json->channels_arr[k].channel_id);
         json->channels_arr[k].user_ids_size = mx_list_size(user_id);
         json->channels_arr[k].user_ids = malloc(sizeof(int) * json->channels_arr[k].user_ids_size);
         j = 0;
         while (user_id != NULL) {
             json->channels_arr[k].user_ids[j] = mx_atoi(user_id->data);
             user_id = user_id->next;
             j++;
         }
         mx_get_channel_name_from_database_channels(json, k);
         mx_del_list(user_id, json->channels_arr[k].user_ids_size);
         user_id = NULL;
         channel_id = channel_id->next;
         k++;
    }
    mx_del_list(channel_id, json->channels_arr_size);
    mx_strdel(&where);
    mx_del_strarr(&fill);
}

static void json_login_success(t_list *data, t_clients *client) {
    char *new_json = NULL;

    t_json_data json = {.type = JS_LOG_IN,
            .status = 200,
            .user_id = mx_atoi(data->data)};
    mx_strcpy(json.pers_info.first_name, data->next->data);
    mx_strcpy(json.pers_info.last_name, data->next->next->data);
    mx_strcpy(json.token, client->token);
    mx_fill_channels(&json);
    client->first->index++;
    new_json = mx_json_make_json(JS_LOG_IN, &json);
    mx_logger("JSON write",  new_json + 4);
    SSL_write(client->ssl, new_json , strlen(new_json + 4) + 4);
    mx_strdel(&new_json);
    for(int i = 0; i < json.channels_arr_size; i++) {
        free(json.channels_arr[i].user_ids);
    }
    free(json.channels_arr);
}

static void json_login_unauthorized(t_clients *client) {
    t_json_data json = {.type = JS_LOG_IN, .status = 401};
    char *new_json = mx_json_make_json(JS_LOG_IN, &json);

    mx_logger("JSON write:",  new_json + 4);
    SSL_write(client->ssl, new_json, mx_strlen(new_json + 4) + 4);
    mx_strdel(&new_json);
}

static void json_login_incorrectly_filled_fields(t_clients *client) {
    t_json_data json = {.type = JS_LOG_IN, .status = 412};
    char *new_json = mx_json_make_json(JS_LOG_IN, &json);

    mx_logger("JSON write:",  new_json + 4);
    SSL_write(client->ssl, new_json, mx_strlen(new_json + 4) + 4);
    mx_strdel(&new_json);
}

void mx_controller_login(t_json_data *json, t_clients *client) {
    char *where = NULL;
    char **fill = NULL;
    t_list *data = NULL;

    if (mx_valid_login(json) == false) {
        json_login_incorrectly_filled_fields(client);
        return;
    }
    fill = mx_model_user_fill_table();
    asprintf(&where, "%s='%s' AND %s='%s'",
             fill[3],
             json->pers_info.login,
             fill[4],
             mx_hmac_sha_256(json->pers_info.login, json->pers_info.password));
    data = mx_read_database(mx_model_user_database(), mx_model_user_name_table(), "*", where);
    if (data != NULL) {
        client->user_id = mx_atoi(data->data);
        client->token =  mx_insert_token(fill, client->user_id);
        json_login_success(data, client);
        mx_strdel(&client->token);
    } else
        json_login_unauthorized(client);
    mx_del_list(data, mx_list_size(data));
    mx_strdel(&where);
    mx_del_strarr(&fill);
}
