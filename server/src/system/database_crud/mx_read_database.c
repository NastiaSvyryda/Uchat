#include "uchat_server.h"

static int callback(void *data, int argc, char **argv, char **azColName) {
    t_list **list_data = (t_list**)data;
    (void)azColName;

    if(argc == 0)
        return 0;
    for (int i = 0; i < argc; ++i)
        mx_push_back(*(&list_data), mx_strdup(argv[i]));
    return 0;
}

t_list *sqlite3_database__read(char *table, char *fill_table, char *where) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open(mx_config_sqlite3_db_name(), &db);
    char *sql;
    t_list *list = NULL;

    mx_valid_sqlite3_open_db(rc, db);
    if (where == NULL)
        asprintf(&sql, "SELECT %s FROM %s;", fill_table, table);
    else
        asprintf(&sql, "SELECT %s FROM %s WHERE %s;", fill_table, table, where);
    mx_logger("read_database sql", sql);
    rc = sqlite3_exec(db, sql, callback, &list, &err_msg);
    mx_valid_sqlite3_failed_data(rc, db, err_msg);
    mx_strdel(&sql);
    sqlite3_close(db);
    return list;
}

t_list *mx_read_database(char *database, char *table, char *fill_table, char *where) {
    t_list *data = NULL;

    if (strcmp(database, "sqlite3") == 0)
        data =  sqlite3_database__read(table, fill_table, where);
    return data;
}
