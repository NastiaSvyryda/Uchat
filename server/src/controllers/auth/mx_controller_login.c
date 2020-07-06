#include "uchat_server.h"

void mx_controller_login(t_json_data *json, t_clients *client) {
    mx_printstr("controller\n");

    if(strcmp(json->pers_info.login, "123") == 0 && strcmp(json->pers_info.password, "123") == 0) {

        write(client->fd, "success", mx_strlen("success"));
        mx_printint(client->fd);
        mx_printstr(json->pers_info.login);
        mx_printstr(json->pers_info.password);
    }
}
