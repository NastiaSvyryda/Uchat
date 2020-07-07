#include "uchat_server.h"

static void sqlite3_database_sql(char *table, char *fill_table, char *value_table) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open(mx_config_sqlite3_db_name(), &db);
    char *sql;

    mx_valid_sqlite3_open_db(rc, db);
    asprintf(&sql, "INSERT INTO %s ( %s ) VALUES ( %s );",
             table, fill_table, value_table);
    mx_printstr(sql);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    mx_valid_sqlite3_failed_data(rc, db, err_msg);
    mx_strdel(&sql);
    sqlite3_close(db);
}

void mx_create_databases(char *database, char *table, char *fill_table, char *value_table) {
    if (strcmp(database, "sqlite3") == 0)
        sqlite3_database_sql(table, fill_table, value_table);
}
