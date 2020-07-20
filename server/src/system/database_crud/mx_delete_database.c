#include "uchat_server.h"

void sqlite3_database_sql(char *table, char *where) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open(mx_config_sqlite3_db_name(), &db);
    char *sql;

    mx_valid_sqlite3_open_db(rc, db);
    asprintf(&sql, "DELETE FROM %s WHERE %s;",
             table, where);
    mx_logger("delete_databases sql", sql);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    mx_valid_sqlite3_failed_data(rc, db, err_msg);
    mx_strdel(&sql);
    sqlite3_close(db);
}

void mx_delete_database(char *database, char *table, char *where) {
    if (strcmp(database, "sqlite3") == 0)
        sqlite3_database_sql(table, where);

}
