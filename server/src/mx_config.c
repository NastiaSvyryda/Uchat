#include "uchat_server.h"

char *mx_config_sqlite3_db_name(void) {
    return "database/database.db";
}

int mx_config_pagination(void) {
    return 50;
}

char *mx_config_ssl_file_name(void) {
    return "mycert.pem";
}
