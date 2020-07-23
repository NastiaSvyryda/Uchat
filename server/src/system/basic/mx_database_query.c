#include "uchat_server.h"

t_database_query *mx_database__query_create() {
    t_database *database_query = malloc(sizeof(t_clients));
    database->where = NULL;
    database->fill_table = NULL;
    database->model_fill_table = NULL;
    database->list = NULL;
    return database_query;
}

void mx_database_query_clean(t_database_query **database_query) {
    mx_strdel(&(*database_query)->where);
    mx_strdel(&(*database_query)->fill_table);
    mx_del_strarr(&(*database_query)->model_fill_table);
    mx_del_list((*database_query)->list, mx_list_size((*database_query)->list));
}
