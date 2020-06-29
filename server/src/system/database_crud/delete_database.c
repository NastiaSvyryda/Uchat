#include "uchat_server.h"

void sqlite3_database_sql(char *table, char *fill_table, char *where) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open(config_sqlite3_db_name(), &db);
    valid_sqlite3_open_db(rc, db);
    char *sql;

    valid_sqlite3_open_db(rc, db);
    asprintf(&sql, "DELETE FROM %s ( %s ) WHERE %s;",
             table, fill_table, where);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    valid_sqlite3_failed_data(rc, db, err_msg);
    sqlite3_close(db);
}

void delete_database(char *database, char *table, char *fill_table, char *where) {
    if (strcmp(database, "sqlite3") == 0)
        sqlite3_database_sql(table, fill_table, where);

}
