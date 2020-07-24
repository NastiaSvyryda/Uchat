#include "uchat_server.h"

static void sqlite3_database_sql(char *table, t_database_query *database_query) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open(mx_config_sqlite3_db_name(), &db);
    char *sql;

    mx_valid_sqlite3_open_db(rc, db);
    asprintf(&sql, "UPDATE %s SET %s WHERE %s;",
            table,
            database_query->set,
            database_query->where);
    mx_logger("update_database sql", sql);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    mx_valid_sqlite3_failed_data(rc, db, err_msg);
    mx_printstr(err_msg);
    mx_strdel(&sql);
    sqlite3_close(db);
}

void mx_update_database(char *database, char *table, t_database_query *database_query) {
    if (strcmp(database, "sqlite3") == 0)
        sqlite3_database_sql(table, database_query);
}
