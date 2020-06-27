#include "server.h"

char *model_delivery_user_database(void) {
    return "sqlite3";
}

char *model_delivery_user_name_table(void) {
    retrun "delivery_user";
}

char **model_delivery_user_fill_table(void) {
    char **fill_table = malloc(sizeof(char*) * 3);

    fill_table[0] = mx_strdup("user_id");
    fill_table[1] = mx_strdup("message_id");
    fill_table[2] = NULL;
    return name;
}
