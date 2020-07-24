#include "uchat_server.h"

void mx_controller_register(t_json_data *json, t_main *main) {
    t_database_query *db = mx_database_query_create();

    if (mx_valid_register(json) == false) {
        mx_res_js_register_incorrectly_filled_fields(main);
        return;
    }
    db->model_fill_table = mx_model_user_fill_table();
    asprintf(&db->fill_table, "%s, %s, %s, %s, %s",
             db->model_fill_table[1],
             db->model_fill_table[2],
             db->model_fill_table[3],
             db->model_fill_table[4],
             db->model_fill_table[6]);
    if ((mx_valid_is_login(db->model_fill_table[3], json)) == false) {
        asprintf(&db->value, "\"%s\", \"%s\", \"%s\", \"%s\", %s",
                 json->pers_info.first_name,
                 json->pers_info.last_name,
                 json->pers_info.login,
                 mx_hmac_sha_256(json->pers_info.login, json->pers_info.password),
                 "datetime('now')");
        main->client->user_id = mx_create_databases(mx_model_user_database(), mx_model_user_name_table(), db);
        main->client->token = mx_insert_token(db->model_fill_table, main->client->user_id);
        mx_res_js_register_success(main, json);
        mx_strdel(&main->client->token);
    } else
        mx_res_js_register_incorrectly_filled_fields(main);
    mx_database_query_clean(&db);
}
