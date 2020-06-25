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
typedef struct s_clients {
    struct s_clients *next;
    int fd;
    char *name_to;
    char *name_from;
    struct s_clients *first;
}t_clients;

void *cycle(void *newfd) {
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
            mx_printstr("\npuk1\n");
            //исправить сегфоулт 44 строка нужно продебажить
            if (strcmp(fd->name_from, fd_f->name_to) == 0 && strcmp(fd_f->name_to, "all") != 0) {
                mx_printstr("\npuk2\n");
                write(fd->fd, buff, mx_strlen(buff));
                mx_printstr("message delivered to ");
                mx_printstr(fd->name_to);
                mx_printchar('\n');
                break;
            }
            else if (strcmp(fd->name_to, "all") == 0) {
                write(fd->fd, buff, mx_strlen(buff));
                mx_printstr("message delivered to ");
                mx_printstr(fd->name_to);
                mx_printchar('\n');
                fd = fd->next;
            }
            else
                fd = fd->next;
        }
        memset(buff, '\0', 1024);
        memset(name, '\0', 100);
    }
    return NULL;
}


int main(int argc, char **argv) {
    t_clients *client = malloc(sizeof(t_clients));
    client->first = client;
    client->next = NULL;
    client->fd = 0;
    char buff[100];
//    int i = 0;
    int listenfd = 0;
    int cli_size;
    struct sockaddr_in serv;
    struct sockaddr_in cli;
    pthread_t thread = NULL;
    int status = 0;

    if (argc != 2) {
        mx_printerr("uchat_server: error args\n");
        mx_printerr("example: ./uchat_server port\n");
        exit(1);
    }

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        mx_printerr("uchat: couldn't create socket");
        exit(1);
    }
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(atoi(argv[1]));
    if (bind(listenfd, (struct sockaddr *) &serv, sizeof(serv)) < 0) {
        mx_printerr("uchat_server: couldn't connect socket with ip or port");
        exit(1);
    }
    if (listen(listenfd, 100) < 0) {
        mx_printerr("uchat_server: couldn't listen for connections");
        exit(1);
    }
    cli_size = sizeof(cli);
    while (1) {
        if ((client->fd = accept(listenfd, (struct sockaddr *) &cli,
                (socklen_t *)&cli_size)) == -1) {
            mx_printerr(
                    "uchat_server: error accepting connection on a socket");
            exit(1);
        }
        memset(buff, '\0', 100);
        read(client->fd, buff, 100);
        mx_printstr("name recived\n");
        client->name_from = mx_strdup(buff);
        if ((status = pthread_create(&thread, NULL, cycle, client)) != 0) {
            mx_printerr("uchat_server: thread creating error");
            exit(1);
        }

        printf("%s ", "\033[0;32mUser connected from ip:\033[0;32m");
        printf("%s\n", inet_ntoa(cli.sin_addr));
        client->next = malloc(sizeof(t_clients));
        client->next->first = client->first;
        client = client->next;
    }
}
