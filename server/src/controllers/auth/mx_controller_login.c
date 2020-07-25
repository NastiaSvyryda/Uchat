#include "uchat_server.h"

static void mx_get_channel_name_from_database_channels(t_json_data *json, int index) {
    t_database_query *db = mx_database_query_create();

    db->model_fill_table = mx_model_channel_fill_table();
    asprintf(&db->fill_table, "%s", db->model_fill_table[1]);
    asprintf(&db->where, "%s=%d",
             db->model_fill_table[0],
             json->channels_arr[index].channel_id);
    db->list = mx_read_database(mx_model_channel_database(), mx_model_channel_name_table(), db);
    strcpy(json->channels_arr[index].channel_name, db->list->data);
    mx_database_query_clean(&db);
}

static void mx_fill_channels(t_json_data *json) {
    t_database_query *db = mx_database_query_create();

    t_list *channel_id = NULL;
    t_list *user_id = NULL;
    int k = 0;
    int j = 0;
    db->model_fill_table = mx_model_user_channel_fill_table();
    asprintf(&db->fill_table ,"%s", "channel_id");
    asprintf(&db->where ,"%s=%d",
             db->model_fill_table[1],
             json->user_id);
    channel_id = mx_read_database(mx_model_user_channel_database(), mx_model_user_channel_name_table(), db);
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
    mx_database_query_clean(&db);
}

static void json_login_success(t_list *data, t_main *main) {
    char *new_json = NULL;

    t_json_data json = {.type = JS_LOG_IN,
            .status = 200,
            .user_id = mx_atoi(data->data)};
    mx_strcpy(json.pers_info.first_name, data->next->data);
    mx_strcpy(json.pers_info.last_name, data->next->next->data);
    strcpy(json.pers_info.login, data->next->next->next->data);
    mx_strcpy(json.token, main->client->token);
    mx_fill_channels(&json);
    new_json = mx_json_make_json(JS_LOG_IN, &json);
    mx_logger("JSON write",  new_json + 4);
    SSL_write(main->client->ssl, new_json , *(int *)new_json + 4);
    main->wait = main->wait->first;
    if (main->wait != NULL) {
        while (main->wait->json_str != NULL) {
            if (main->wait->user_id == json.user_id) {
                mx_printstr(main->wait->json_str + 4);
                SSL_write(main->client->ssl, main->wait->json_str, *(int *)main->wait->json_str + 4);
            }
            main->wait = main->wait->next;
        }
        main->wait = main->wait->first;
    }
    mx_strdel(&new_json);
    for(int i = 0; i < json.channels_arr_size; i++) {
        free(json.channels_arr[i].user_ids);
    }
    free(json.channels_arr);
}

void mx_controller_login(t_json_data *json, t_main *main) {
    t_database_query *db = mx_database_query_create();

    if (mx_valid_login(json) == false) {
        mx_res_js_login_incorrectly_filled_fields(main->client);
        return;
    }
    db->model_fill_table = mx_model_user_fill_table();
    asprintf(&db->fill_table, "%s", "*");
    asprintf(&db->where, "%s='%s' AND %s='%s'",
             db->model_fill_table[3],
             json->pers_info.login,
             db->model_fill_table[4],
             mx_hmac_sha_256(json->pers_info.login, json->pers_info.password));
    db->list = mx_read_database(mx_model_user_database(), mx_model_user_name_table(), db);
    if (db->list != NULL) {
        if(mx_valid_login_auth(db->list) == true) {
            mx_res_js_login_auth(main);
            return;
        }
        main->client->user_id = mx_atoi(db->list->data);
        main->client->token =  mx_insert_token(db->model_fill_table, main->client->user_id);
        json_login_success(db->list, main);

        mx_strdel(&main->client->token);
    } else
        mx_res_js_login_unauthorized(main);
    mx_database_query_clean(&db);
}
