#include "uchat_server.h"

void mx_controller_message(t_clients *client, t_clients *cur_client, t_json_data *json) {
    cur_client->name_to = json->message.client2_id;
    t_message *message = malloc(sizeof(t_message));
    char *json_str = NULL;

    while (client->next != NULL) {
        if (cur_client->name_to != 3 &&
            client->name_from == cur_client->name_to) {
            message->text = mx_strdup(json->message.text);
            json_str = mx_json_message_in_request(json);
            write(client->fd, message->text, mx_strlen(message->text));
            mx_printstr("message delivered to ");
            mx_printint(cur_client->name_to);
            mx_printchar('\n');
            break;
        }

        client = client->next;
    }
    mx_strdel(&message->text);
    free(message);
    message = NULL;
}
