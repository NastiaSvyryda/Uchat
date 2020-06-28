#include "server.h"

static void sqlite3(char *table, char *fill_table, char *value_table) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open(config_sqlite3_db_name(), &db);
    valid_sqlite3_open_db(rc, db);
    char *sql_create

    valid_sqlite3_open_db(rc, db);
    asprintf(sql_create, "INSERT INTO %s ( %s ) VALUES ( %s );",
             table, fill_table, value_table);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    valid_sqlite3_failed_data(rc, db, err_msg);
    sqlite3_close(db);
}

void create_databases(char *database, char *table, char *fill_table, char *value_table) {
    if (strcmp(database, "sqlite3") == 0)
        sqlite3(table, fill_table, value_table);
}