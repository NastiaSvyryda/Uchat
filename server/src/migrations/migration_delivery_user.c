#include "server.h"

static void create_table_for_sqlite3(void) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open(config_sqlite3_db_name(), &db);
    char *sql_create

    valid_sqlite3_open_db(rc, db);
    asprintf(sql_create, "CREATE TABLE %s (\n"
                         "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\n"
                         "user_id INTEGER NOT NULL, \n"
                         "message_id INTEGER NOT NULL,\n"
                         "FOREIGN KEY(user_id) REFERENCES %s(id), \n"
                         "FOREIGN KEY(message_id) REFERENCES messages(id) );",
                         model_delivery_user_name_table(), model_user_name_table(), model_message_name_table());
    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    valid_sqlite3_failed_data(rc, db, err_msg);
    sqlite3_close(db);
}

static void delete_table_for_sqlite3(void) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open(config_sqlite3_db_name(), &db);
    char *sql_create

    valid_sqlite3_open_db(rc, db);
    asprintf(sql_create, "DROP TABLE %s ;", model_delivery_user_name_table());
    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    valid_sqlite3_failed_data(rc, db, err_msg);
    sqlite3_close(db);
}

void migration_delivery_user(bool status) {
    if (mx_strcmp(model_delivery_user_database(), "sqlite3") == 0 && status == true)
        create_table_for_sqlite3();
    else if (mx_strcmp(model_delivery_user_database(), "sqlite3") == 0 && status == false)
        delete_table_for_sqlite3();
}
