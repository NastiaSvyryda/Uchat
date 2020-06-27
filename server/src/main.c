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
#include <pthread.h>

typedef struct s_clients {
    struct s_clients *next;
    int fd;
    char *name_to;
    char *name_from;
    struct s_clients *first;
}t_clients;

void *send_message(void *newfd) {
    char buff[1024];
    char name[100];
    t_clients *fd = (t_clients *)newfd;
    int curr_fd = fd->fd;
    t_clients *fd_f = fd->first;
    while(1) {
        fd = fd_f;

        read(curr_fd, name, 100);
        printf("name to recived %s\n", name);
        fd->name_to = mx_strdup(name);
        read(curr_fd, buff, 1024);
        printf("message recived %s\n", buff);
        // fd = fd->next;
        mx_printstr(buff);
        mx_printchar('\n');
//        scanf("%s", buff);
        while (fd != NULL) {
            //исправить сегфоулт 44 строка нужно продебажить  && strcmp(fd_f->name_to, "all") != 0
            if (strcmp(fd_f->name_to, "all") != 0 &&
                strcmp(fd->name_from, fd_f->name_to) == 0) {
                write(fd->fd, buff, mx_strlen(buff));
                mx_printstr("message delivered to ");
                mx_printstr(fd_f->name_to);
                mx_printchar('\n');
                mx_strdel(&fd_f->name_to);
                break;
            } else if (strcmp(fd_f->name_to, "all") == 0) {
                write(fd->fd, buff, mx_strlen(buff));
                mx_printstr("message delivered to ");
                mx_printstr(fd->name_to);
                mx_printchar('\n');
                fd = fd->next;
            } else
                fd = fd->next;
        }
        memset(buff, '\0', 1024);
        memset(name, '\0', 100);
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
    mx_printint(*fd);
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

    if ((pthread_create(&thread, NULL, send_message, client)) != 0) {
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
    client->name_to = NULL;
    client->name_from = NULL;
    return  client;
}
static void next_list_elem (t_clients *client) {
    client->next = malloc(sizeof(t_clients));
    client->next->first = client->first;
    client = client->next;
}
static void get_client_name(t_clients *client) {
    char buff[100];

    memset(buff, '\0', 100);
    read(client->fd, buff, 100);
    mx_printstr("name recived\n");
    client->name_from = mx_strdup(buff);
}

int main(int argc, char **argv) {
    t_clients *client = create_clients();
    int listenfd = 0;
    struct sockaddr_in cli;

    check_argc_error(argc);
    conn_list_sock(&listenfd, argv);
    while (1) {
        cli = accept_connections(client, listenfd);
        get_client_name(client);
        thread_create(client, cli);
        next_list_elem(client);
    }
}

