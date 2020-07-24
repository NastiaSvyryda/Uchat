#include "uchat_server.h"

static bool check_channels (t_json_data *json) {
    t_database_query *db = mx_database_query_create();
    bool status = true;

    db->model_fill_table = mx_model_channel_fill_table();
    asprintf(&db->fill_table, "%s", db->model_fill_table[0]);
    asprintf(&db->where, "%s='%s' AND %s=%d",
            db->model_fill_table[1],
            json->new_channel_data.channel_name,
            db->model_fill_table[2],
            json->message.client1_id);
    db->list = mx_read_database(mx_model_channel_database(), mx_model_channel_name_table(), db->fill_table, db->where);
    if (db->list == NULL)
        status = false;
    mx_database_query_clean(&db);
    return status;
}

static bool check_users_in_channels (t_json_data *json, int index) {
    t_database_query *db = mx_database_query_create();
    bool status = true;

    db->model_fill_table = mx_model_user_channel_fill_table();
    asprintf(&db->fill_table, "%s", db->model_fill_table[0]);
    asprintf(&db->where, "%s=%d AND %s=%d",
             db->model_fill_table[1],
             json->new_channel_data.user_ids[index],
             db->model_fill_table[2],
             json->new_channel_data.channel_id);
    db->list = mx_read_database(mx_model_user_channel_database(), mx_model_user_channel_name_table(), db->fill_table, db->where);
    if (db->list == NULL)
        status = false;
    mx_database_query_clean(&db);
    return status;
}

void mx_get_channel_id_from_database_channels(t_json_data *json) {
    t_database_query *db = mx_database_query_create();

    db->model_fill_table = mx_model_channel_fill_table();
    asprintf(&db->fill_table, "%s", db->model_fill_table[0]);
    asprintf(&db->where,"%s='%s' AND %s=%d",
             db->model_fill_table[1],
             json->new_channel_data.channel_name,
             db->model_fill_table[2],
             json->message.client1_id);
    db->list = mx_read_database(mx_model_channel_database(), mx_model_channel_name_table(), db->fill_table, db->where);
    json->new_channel_data.channel_id = mx_atoi(db->list->data);
    mx_database_query_clean(&db);
}

static void fill_database_channels(t_json_data *json) {
    t_database_query *db = mx_database_query_create();

    if (check_channels(json) == false) {
        db->model_fill_table = mx_model_channel_fill_table();
        asprintf(&db->fill_table, "%s, %s",
                 db->model_fill_table[1],
                 db->model_fill_table[2]);
        asprintf(&db->value, "\"%s\", %d",
                 json->new_channel_data.channel_name,
                 json->message.client1_id);
        json->new_channel_data.channel_id = mx_create_databases(mx_model_channel_database(), mx_model_channel_name_table(), db->fill_table, db->value);
        mx_database_query_clean(&db);
    }
    else
        mx_get_channel_id_from_database_channels(json);
    json->message.channel_id = json->new_channel_data.channel_id;
}


static void fill_database_user_channel(t_json_data *json) {
    t_database_query *db = NULL;

    for (int i = 0; i < json->new_channel_data.user_ids_size; i++) {
        if (check_users_in_channels(json, i) == false) {
            db = mx_database_query_create();
            db->model_fill_table = mx_model_user_channel_fill_table();
            asprintf(&db->fill_table, "%s, %s, %s",
                     db->model_fill_table[1],
                     db->model_fill_table[2],
                     db->model_fill_table[3]);
            asprintf(&db->value, "%d, %d, %s",
                     json->new_channel_data.user_ids[i],
                     json->new_channel_data.channel_id,
                     "datetime('now')");
            mx_create_databases(mx_model_user_channel_database(), mx_model_user_channel_name_table(), db->fill_table, db->value);
            mx_database_query_clean(&db);
        }
    }
}

void mx_controller_new_channel(t_json_data *json) {
    fill_database_channels(json);
    fill_database_user_channel(json);
}
