#include "uchat_server.h"

void mx_valid_sqlite3_failed_data(int rc, sqlite3 *db, char *err_msg) {
    if (rc != SQLITE_OK ) {

        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(1);
    }
}
