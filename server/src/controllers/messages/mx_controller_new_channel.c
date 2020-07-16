#include "uchat_server.h"

static bool check_channels (t_json_data *json) {
    char *where = NULL;
    char **fill = NULL;
    t_list *data = NULL;
    bool ret;
    fill = mx_model_channel_fill_table();
    asprintf(&where, "%s='%s' AND %s=%d",
            fill[1],
            json->new_channel_data.channel_name,
            fill[2],
            json->message.client1_id);
    data = mx_read_database(mx_model_channel_database(), mx_model_channel_name_table(), "id", where);
    if (data == NULL)
        ret = false;
    else
        ret = true;
    mx_strdel(&where);
    mx_del_strarr(&fill);
    mx_del_list(data, mx_list_size(data));
    return ret;
}

static bool check_users_in_channels (t_json_data *json, int index) {
    char *where = NULL;
    char **fill = NULL;
    t_list *data = NULL;
    bool ret;

    fill = mx_model_user_channel_fill_table();
    asprintf(&where, "%s=%d AND %s=%d",
             fill[1],
             json->new_channel_data.user_ids[index],
             fill[2],
             json->new_channel_data.channel_id);
    data = mx_read_database(mx_model_user_channel_database(), mx_model_user_channel_name_table(), "id", where);
    if (data == NULL)
        ret = false;
    else
        ret = true;
    mx_strdel(&where);
    mx_del_strarr(&fill);
    mx_del_list(data, mx_list_size(data));
    return ret;
}
void mx_get_channel_id_from_database_channels(t_json_data *json) {
    char *where = NULL;
    char **fill = NULL;
    t_list *data = NULL;

    fill = mx_model_channel_fill_table();
    asprintf(&where ,"%s='%s' AND %s=%d",
             fill[1],
             json->new_channel_data.channel_name,
             fill[2],
             json->message.client1_id);
    data = mx_read_database(mx_model_channel_database(), mx_model_channel_name_table(), "id", where);
    json->new_channel_data.channel_id = mx_atoi(data->data);
    mx_strdel(&where);
    mx_del_strarr(&fill);
    mx_del_list(data, mx_list_size(data));
}


static void fill_database_channels(t_json_data *json) {
    char *fill_str = NULL;
    char **fill_table = NULL;
    char *value_str = NULL;

    if (check_channels(json) == false) {
        fill_table = mx_model_channel_fill_table();
        asprintf(&fill_str, "%s, %s",
                 fill_table[1],
                 fill_table[2]);
        asprintf(&value_str, "\"%s\", %d",
                 json->new_channel_data.channel_name,
                 json->message.client1_id);
        json->new_channel_data.channel_id = mx_create_databases(mx_model_channel_database(), mx_model_channel_name_table(), fill_str, value_str);
        mx_del_strarr(&fill_table);
        mx_strdel(&value_str);
        mx_strdel(&fill_str);
    }
    else
        mx_get_channel_id_from_database_channels(json);
    json->message.channel_id = json->new_channel_data.channel_id;
}


static void fill_database_user_channel(t_json_data *json) {
    char *fill_str = NULL;
    char **fill_table = NULL;
    char *value_str = NULL;


    mx_printint(json->new_channel_data.user_ids_size);
    for (int i = 0; i < json->new_channel_data.user_ids_size; i++) {
        if (check_users_in_channels(json, i) == false) {
            fill_table = mx_model_user_channel_fill_table();
            asprintf(&fill_str, "%s, %s, %s",
                     fill_table[1],
                     fill_table[2],
                     fill_table[3]);
            asprintf(&value_str, "%d, %d, %s",
                     json->new_channel_data.user_ids[i],
                     json->new_channel_data.channel_id,
                     "datetime('now')");
            mx_create_databases(mx_model_user_channel_database(), mx_model_user_channel_name_table(), fill_str,
                                value_str);
            mx_del_strarr(&fill_table);
            mx_strdel(&value_str);
            mx_strdel(&fill_str);
        }
    }
}

void mx_controller_new_channel(t_json_data *json) {
    fill_database_channels(json);
    fill_database_user_channel(json);
}
