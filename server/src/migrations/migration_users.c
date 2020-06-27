#include "serve.h"

static create_for_sqlite3() {
    char *sql_create = "CREATE TABLE users (\n"
                       "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\n"
                       "first_name VARCHAR(255) NOT NULL,\n"
                       "last_name VARCHAR(255) NOT NULL,\n"
                       "login VARCHAR(255) NOT NULL,\n"
                       "password VARCHAR(255) NOT NULL,\n"
                       "token VARCHAR(255) NULL,\n"
                       "create_at DATETIME NOT NULL )";

}

