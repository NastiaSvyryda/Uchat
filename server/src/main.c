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
    struct s_clients *first;
}t_clients;

void *cycle(void *newfd) {
    char buff[1024];
    t_clients *fd = (t_clients *)newfd;
    t_clients *fd_f = fd->first;
    while(1) {
        fd = fd_f;
        read(fd->fd, buff, 1024);
        fd = fd->next;
        mx_printstr("message recived");
//        scanf("%s", buff);
        while (fd != NULL) {
            write(fd->fd, buff, mx_strlen(buff));
            mx_printstr("message delivered");
            fd = fd->next;
        }
        memset(buff, '\0', 1024);
    }
    return NULL;
}


int main(int argc, char **argv) {
    t_clients *client = malloc(sizeof(t_clients));
    client->first = client;
    client->next = NULL;
    client->fd = 0;
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
        int a = 0;
        if ((a = accept(listenfd, (struct sockaddr *) &cli,
                (socklen_t *)&cli_size)) == -1) {
            mx_printerr(
                    "uchat_server: error accepting connection on a socket");
            exit(1);
        }
        client->fd = a;
        mx_printint(client->fd);
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
