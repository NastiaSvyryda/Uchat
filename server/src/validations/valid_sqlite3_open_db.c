#include "server.h"

void valid_sqlite3_open_db(int rc, sqlite3 *db) {
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n",
                sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
}