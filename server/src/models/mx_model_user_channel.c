#include "uchat_server.h"

char *mx_model_user_channel_database(void) {
    return "sqlite3";
}

char *mx_model_user_channel_name_table(void) {
    return "user_channel";
}

char **mx_model_user_channel_fill_table(void) {
    char **fill_table = malloc(sizeof(char*) * 5);

    fill_table[0] = mx_strdup("id");
    fill_table[1] = mx_strdup("user_id");
    fill_table[2] = mx_strdup("channel_id");
    fill_table[3] = mx_strdup("create_at");
    fill_table[4] = NULL;
    return fill_table;
}

