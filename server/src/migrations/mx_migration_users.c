#include "uchat_server.h"

static void create_table_for_sqlite3(void) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open(mx_config_sqlite3_db_name(), &db);
    char *sql = NULL;
    char **fill = mx_model_user_fill_table();

    mx_valid_sqlite3_open_db(rc, db);
    asprintf(&sql, "CREATE TABLE IF NOT EXISTS %s ("
                       "%s INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
                       "%s VARCHAR(255) NOT NULL,"
                       "%s VARCHAR(255) NOT NULL,"
                       "%s VARCHAR(255) NOT NULL,"
                       "%s VARCHAR(255) NOT NULL,"
                       "%s VARCHAR(255) NULL,"
                       "%s DATETIME NOT NULL);",
                       mx_model_user_name_table(),
                       fill[0],
                       fill[1],
                       fill[2],
                       fill[3],
                       fill[4],
                       fill[5],
                       fill[6]);
    mx_printstr(sql);
    mx_printstr("\n");
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    mx_valid_sqlite3_failed_data(rc, db, err_msg);
    mx_strdel(&sql);
    mx_del_strarr(&fill);
    sqlite3_close(db);
}

static void delete_table_for_sqlite3(void) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open(mx_config_sqlite3_db_name(), &db);
    char *sql;

    mx_valid_sqlite3_open_db(rc, db);
    asprintf(&sql, "DROP TABLE IF EXISTS %s;", mx_model_user_name_table());
    mx_printstr(sql);
    mx_printstr("\n");
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    mx_valid_sqlite3_failed_data(rc, db, err_msg);
    mx_strdel(&sql);
    sqlite3_close(db);
}

void mx_migration_users(bool status) {
    if ((mx_strcmp(mx_model_user_database(), "sqlite3") == 0) && (status == true))
        create_table_for_sqlite3();
    else if ((mx_strcmp(mx_model_user_database(), "sqlite3") == 0) && (status == false))
        delete_table_for_sqlite3();
}
