#include "uchat_server.h"

static void sqlite3_database_sql(char *table, char *set, char *where) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open(config_sqlite3_db_name(), &db);
    valid_sqlite3_open_db(rc, db);
    char *sql;

    valid_sqlite3_open_db(rc, db);
    asprintf(&sql, "UPDATE %s SET %s  WHERE %s;",
             table, set, where);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    valid_sqlite3_failed_data(rc, db, err_msg);
    sqlite3_close(db);
}

void mx_update_database(char *database, char *table, char *set, char *where) {
    if (strcmp(database, "sqlite3") == 0)
        sqlite3_database_sql(table, set, where);
}
