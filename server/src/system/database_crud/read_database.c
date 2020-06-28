#include "server.h"

static int sqlite3_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    return 0;
}

static void sqlite3(char *table, char *fill_table, char *where) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open(config_sqlite3_db_name(), &db);
    valid_sqlite3_open_db(rc, db);
    char *sql_create

    valid_sqlite3_open_db(rc, db);
    if (where == NULL) {
        asprintf(sql_create, "SELECT %s FROM %s",
                fill_table, table);
    } else {
        asprintf(sql_create, "SELECT %s FROM %s WHERE %s",
                 fill_table, table, where);
    }
    rc = sqlite3_exec(db, sql, sqlite3_callback, 0, &err_msg);
    valid_sqlite3_failed_data(rc, db, err_msg);
    sqlite3_close(db);
}

void read_database(char *database, char *table, char *set, char *where) {
    if (strcmp(database, "sqlite3") == 0)
        sqlite3(table, set, where);
}