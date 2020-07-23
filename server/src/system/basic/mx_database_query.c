#include "uchat_server.h"

t_database_query *mx_database_query_create() {
    t_database_query *database_query = malloc(sizeof(t_database_query));

    database_query->fill_table = NULL;
    database_query->model_fill_table = NULL;
    database_query->set = NULL;
    database_query->where = NULL;
    database_query->value = NULL;
    database_query->list = NULL;
    return database_query;
}

void mx_database_query_clean(t_database_query **database_query) {
    if(database_query != NULL && *database_query != NULL) {
        if((*database_query)->fill_table != NULL)
            mx_strdel(&(*database_query)->fill_table);
        if((*database_query)->model_fill_table != NULL)
            mx_del_strarr(&(*database_query)->model_fill_table);
        if((*database_query)->value != NULL)
            mx_strdel(&(*database_query)->value);
        if((*database_query)->set != NULL)
            mx_strdel(&(*database_query)->set);
        if((*database_query)->where != NULL)
            mx_strdel(&(*database_query)->where);
        if((*database_query)->list != NULL)
            mx_del_list((*database_query)->list, mx_list_size((*database_query)->list));
        free(*database_query);
    }
}

