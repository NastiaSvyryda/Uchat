#include "uchat_server.h"

static int sqlite3_database_sql(char *table, char *fill_table, char *value_table) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open(mx_config_sqlite3_db_name(), &db);
    char *sql;
    int last_id;

    mx_valid_sqlite3_open_db(rc, db);
    asprintf(&sql, "INSERT INTO %s ( %s ) VALUES ( %s );",
             table, fill_table, value_table);
    mx_printstr(sql);
    mx_printstr("\n");
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    mx_valid_sqlite3_failed_data(rc, db, err_msg);
    last_id = sqlite3_last_insert_rowid(db);
    mx_strdel(&sql);
    sqlite3_close(db);
    return last_id;
}

int mx_create_databases(char *database, char *table, char *fill_table, char *value_table) {
    if (strcmp(database, "sqlite3") == 0)
        return sqlite3_database_sql(table, fill_table, value_table);
    return INT_MIN;
}
