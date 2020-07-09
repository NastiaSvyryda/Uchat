#include "uchat_server.h"

char *mx_model_delivery_user_database(void) {
    return "sqlite3";
}

char *mx_model_delivery_user_name_table(void) {
    return "delivery_user";
}

char **mx_model_delivery_user_fill_table(void) {
    char **fill_table = malloc(sizeof(char*) * 4);

    fill_table[0] = mx_strdup("id");
    fill_table[1] = mx_strdup("user_id");
    fill_table[2] = mx_strdup("message_id");
    fill_table[3] = NULL;
    return fill_table;
}

