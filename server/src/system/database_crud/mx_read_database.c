#include "uchat_server.h"

static int callback(void *data, int argc, char **argv, char **azColName) {
    t_list *list = (t_list *)data;
    for (int i = 0; i < argc; i++) {
        mx_push_back(&list, azColName[i]);
        mx_push_back(&list, argv[i]);
    }
    return 0;
}

static void sqlite3_database_sql(char *table, char *fill_table, char *where) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open(config_sqlite3_db_name(), &db);
    valid_sqlite3_open_db(rc, db);
    char *sql;
    t_list *list = NULL;

    valid_sqlite3_open_db(rc, db);
    if (where == NULL)
        asprintf(&sql, "SELECT %s FROM %s", fill_table, table);
    else
        asprintf(&sql, "SELECT %s FROM %s WHERE %s", fill_table, table, where);
    rc = sqlite3_exec(db, sql, callback, &list, &err_msg);
    valid_sqlite3_failed_data(rc, db, err_msg);
    sqlite3_close(db);
}

void mx_read_database(char *database, char *table, char *set, char *where) {
    if (strcmp(database, "sqlite3") == 0)
        sqlite3_database_sql(table, set, where);
}
