#include "uchat_server.h"

static void json_register_incorrectly_filled_fields(t_clients *client) {
    t_json_data json = {.type = JS_REG, .status = 412};
    char *new_json = mx_json_make_json(JS_REG, &json);

    mx_logger("JSON write:",  new_json + 4);
    SSL_write(client->ssl, new_json, mx_strlen(new_json + 4) + 4);
    mx_strdel(&new_json);
}

static void json_register_success(t_clients *client, t_json_data *json_data) {
    char *new_json = NULL;

    t_json_data json = {.type = JS_REG, .status = 200, .user_id = client->user_id};
    strcpy(json.pers_info.first_name, json_data->pers_info.first_name);
    strcpy(json.pers_info.last_name, json_data->pers_info.last_name);
    strcpy(json.pers_info.login, json_data->pers_info.login);
    strcpy(json.token, client->token);
    new_json = mx_json_make_json(JS_REG, &json);
    mx_logger("JSON write:",  new_json + 4);
    SSL_write(client->ssl, new_json, mx_strlen(new_json + 4) + 4);
    mx_strdel(&new_json);
}

static t_list *check_login (char *model_fill_table, t_json_data *json) {
    char *where = NULL;

    asprintf(&where, "%s = '%s'",
             model_fill_table,
            json->pers_info.login);
    return mx_read_database(mx_model_user_database(), mx_model_user_name_table(), model_fill_table, where);
}


void mx_controller_register(t_json_data *json, t_clients *client) {
    t_database_query *db = mx_database_query_create();

    if (mx_valid_register(json) == false) {
        json_register_incorrectly_filled_fields(client);
        return;
    }
    db->model_fill_table = mx_model_user_fill_table();
    asprintf(&db->fill_table, "%s, %s, %s, %s, %s",
             db->model_fill_table[1],
             db->model_fill_table[2],
             db->model_fill_table[3],
             db->model_fill_table[4],
             db->model_fill_table[6]);
    db->list = check_login(db->model_fill_table[3], json);
    if (db->list == NULL) {
        asprintf(&db->value, "\"%s\", \"%s\", \"%s\", \"%s\", %s",
                 json->pers_info.first_name,
                 json->pers_info.last_name,
                 json->pers_info.login,
                 mx_hmac_sha_256(json->pers_info.login, json->pers_info.password),
                 "datetime('now')");
        client->user_id = mx_create_databases(mx_model_user_database(), mx_model_user_name_table(), db->fill_table, db->value);
        client->token = mx_insert_token(db->model_fill_table, client->user_id);
        json_register_success(client, json);
        mx_strdel(&client->token);
    } else
        json_register_incorrectly_filled_fields(client);
    mx_database_query_clean(&db);
}
