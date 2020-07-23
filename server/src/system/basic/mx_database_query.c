#include "uchat_server.h"

t_database_query *mx_database_query_create() {
    t_database_query *database_query = malloc(sizeof(t_database_query));

    database_query->fill_table = NULL;
    database_query->model_fill_table = NULL;
    database_query->where = NULL;
    database_query->value = NULL;
    database_query->list = NULL;
    return database_query;
}

void mx_database_query_clean(t_database_query **database_query) {
    mx_strdel(&(*database_query)->fill_table);
    mx_del_strarr(&(*database_query)->model_fill_table);
    mx_strdel(&(*database_query)->value);
    mx_strdel(&(*database_query)->where);
    mx_del_list((*database_query)->list, mx_list_size((*database_query)->list));
    if(*database_query != NULL)
        free(*database_query);
}
