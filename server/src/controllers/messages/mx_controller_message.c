#include "uchat_server.h"

static void fill_database_message(t_json_data *json) {
    char *fill_str = NULL;
    char **fill_table = NULL;
    char *value_str = NULL;
    fill_table = mx_model_message_fill_table();
    asprintf(&fill_str, "%s, %s, %s, %s",
             fill_table[0],
             fill_table[1],
             fill_table[2],
             fill_table[3]);
    asprintf(&value_str, "\"%d\", \"%s\", \"%s\", \"%d\"",
             json->message.client1_id,
             json->message.text,
             "datetime('now')",
             json->message.client2_id);
    mx_create_databases(mx_model_message_database(), mx_model_message_name_table(), fill_str, value_str);
}


void mx_controller_message(t_clients *client, t_clients *cur_client, t_json_data *json) {
    cur_client->name_to = json->message.client2_id;
    t_message *message = malloc(sizeof(t_message));
    char *json_str = NULL;

    fill_database_message(json);

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
