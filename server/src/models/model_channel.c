#include "uchat_server.h"

char *model_channel_database(void) {
    return "sqlite3";
}

char *model_channel_name_table(void) {
    return "channels";
}

char **model_channel_fill_table(void) {
    char **fill_table = malloc(sizeof(char*) * 3);

    fill_table[0] = mx_strdup("name");
    fill_table[1] = mx_strdup("created_by");
    fill_table[2] = NULL;
    return fill_table;
}
