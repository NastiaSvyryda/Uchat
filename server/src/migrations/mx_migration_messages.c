#include "uchat_server.h"

static void create_table_for_sqlite3(void) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open(mx_config_sqlite3_db_name(), &db);
    char *sql;

    mx_valid_sqlite3_open_db(rc, db);
    asprintf(&sql, "CREATE TABLE IF NOT EXISTS %s (\n"
                         "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\n"
                         "user_id INTEGER NOT NULL,\n"
                         "message TEXT NOT NULL, \n"
                         "create_at DATATIME NOT NULL,\n"
                         "channel_id INTEGER NOT NULL,\n"
                         "FOREIGN KEY(user_id) REFERENCES %s(id), \n"
                         "FOREIGN KEY(channel_id) REFERENCES %s(id) );",
                         mx_model_message_name_table(),
                         mx_model_user_name_table(),
                         mx_model_channel_name_table());
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    mx_valid_sqlite3_failed_data(rc, db, err_msg);
    sqlite3_close(db);
}

static void delete_table_for_sqlite3(void) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open(mx_config_sqlite3_db_name(), &db);
    char *sql;

    mx_valid_sqlite3_open_db(rc, db);
    asprintf(&sql, "DROP TABLE %s ;", mx_model_message_name_table());
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    mx_valid_sqlite3_failed_data(rc, db, err_msg);
    sqlite3_close(db);
}

void mx_migration_messages(bool status) {
    if (mx_strcmp(mx_model_message_database(), "sqlite3") == 0 && status == true)
        create_table_for_sqlite3();
    else if (mx_strcmp(mx_model_channel_name_table(), "sqlite3") == 0 && status == false)
        delete_table_for_sqlite3();
}