#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include "../../libmx/inc/libmx.h"
#include <pthread.h>
#include "../../inc/header.h"

typedef struct s_clients {
    struct s_clients *next;
    int fd;
    int name_to;
    int name_from;
    int index;
    struct s_clients *first;
}t_clients;

static void message(t_clients *client, t_clients *cur_client, t_json_data *json) {
    cur_client->name_to = json->message.client2_id;
    t_message *message = malloc(sizeof(t_message));
    char *json_str = NULL;

    while (client->next != NULL) {
        if (cur_client->name_to != 3 &&
            client->name_from == cur_client->name_to) {
//            message->client1_id = json->data.message.client1_id;
//            message->client2_id = json->data.message.client2_id;
            message->text = mx_strdup(json->message.text);
            json_str = mx_json_message_in_request(json);
//            write(client->fd, json_str, mx_strlen(json_str));
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


void *main_cycle(void *newfd) {
    char json_str[100];
    t_json_data *json = NULL;
    t_clients *client = (t_clients *)newfd;
    int curr_fd = client->fd;
    t_clients *cur_client = client;

    while(1) {
        if (client->first->index > 1) { // количество пользователей
            client = client->first;
            read(curr_fd, json_str, 100);
            json = mx_json_parse(json_str);
            if (json->type == 8)
                message(client, cur_client, json);
            //    логин и регистрация
            //    заполнение user_id

            memset(json_str, '\0', 100);
            mx_strdel(&json->message.text);
            //free(&json->data.message);
            free(json);
        }
    }
    return NULL;
}

static void check_argc_error(int argc) {
    if (argc != 2) {
        mx_printerr("uchat_server: error args\n");
        mx_printerr("example: ./uchat_server port\n");
        exit(1);
    }
}

static void conn_list_sock(int *fd, char **argv) {
    struct sockaddr_in serv;

    if ((*fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        mx_printerr("uchat: couldn't create socket");
        exit(1);
    }
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(atoi(argv[1]));
    if (bind(*fd, (struct sockaddr *) &serv, sizeof(serv)) < 0) {
        mx_printerr("uchat_server: couldn't connect socket with ip or port");
        exit(1);
    }
    if (listen(*fd, 100) < 0) {
        mx_printerr("uchat_server: couldn't listen for connections");
        exit(1);
    }
}

static struct sockaddr_in accept_connections(t_clients *client, int listenfd) {
    struct sockaddr_in cli;
    int cli_size = sizeof(cli);

    if ((client->fd = accept(listenfd, (struct sockaddr *)&cli, (socklen_t *) &cli_size)) == -1) {
        mx_printerr(
                "uchat_server: error accepting connection on a socket");
        exit(1);
    }
    return cli;
}
static void thread_create(t_clients *client, struct sockaddr_in cli) {
    pthread_t thread = NULL;

    if ((pthread_create(&thread, NULL, main_cycle, client)) != 0) {
        mx_printerr("uchat_server: thread creating error");
        exit(1);
    }
    printf("%s ", "\033[0;32mUser connected from ip:\033[0;32m");
    printf("%s\n", inet_ntoa(cli.sin_addr));
}

static t_clients *create_clients() {
    t_clients *client = malloc(sizeof(t_clients));
    client->first = client;
    client->next = NULL;
    client->fd = 0;
    client->name_to = 0;
    client->name_from = 0;
    client->index = 0;
    return  client;
}
static void get_client_name(t_clients *client) {
    char buff[100];

    memset(buff, '\0', 100);
    read(client->fd, buff, 100);
    mx_printstr("name recived\n");
    client->name_from = mx_atoi(buff);
}

int main(int argc, char **argv) {
    t_clients *client = create_clients();
    int listenfd = 0;
    struct sockaddr_in cli;
    check_argc_error(argc);
    conn_list_sock(&listenfd, argv);
    while (1) {
        cli = accept_connections(client, listenfd);
        client->first->index++;
        get_client_name(client);
        thread_create(client, cli);
        client->next = create_clients();//next_list_elem(client);
        client->next->first = client->first;
        client = client->next;
    }
}



//        } else if (cur_client->name_to == 3) {
//            if (cur_client->name_from != client->name_from) {
//                write(client->fd, json->data.message.text, mx_strlen(json->data.message.text));
//                mx_printstr("message delivered to ");
//                mx_printint(client->name_from);
//                mx_printchar('\n');
//            }
//        }
