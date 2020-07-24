#include "uchat_server.h"

static int sqlite3_database_sql(char *table, t_database_query *database_query) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open(mx_config_sqlite3_db_name(), &db);
    char *sql;
    int last_id;

    mx_valid_sqlite3_open_db(rc, db);
    asprintf(&sql, "INSERT INTO %s ( %s ) VALUES ( %s );",
             table,
             database_query->fill_table,
             database_query->value);
    mx_logger("create_databases sql", sql);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    mx_valid_sqlite3_failed_data(rc, db, err_msg);
    last_id = sqlite3_last_insert_rowid(db);
    mx_strdel(&sql);
    sqlite3_close(db);
    return last_id;
}

int mx_create_databases(char *database, char *table, t_database_query *database_query) {
    if (strcmp(database, "sqlite3") == 0)
        return sqlite3_database_sql(table, database_query);
    return INT_MIN;
}
