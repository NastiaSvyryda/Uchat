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

void *cycle(void *newfd) {
    char buff[1024];
    int *x_ptr = (int *)newfd;
    mx_printint(*x_ptr);
    while(1) {
        memset(buff, '\0', 1024);
        mx_printstr("ARNI");
        scanf("%s", buff);

        write(*x_ptr, buff, mx_strlen(buff));
    }
    return NULL;
}

int main(int argc, char **argv) {
    int listenfd = 0;
    int newfd = 0;
    int cli_size;
    struct sockaddr_in serv;
    struct sockaddr_in cli;
    pthread_t thread;
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
    if (bind(listenfd, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
        mx_printerr("uchat_server: couldn't connect socket with ip or port");
        exit(1);
    }
    if (listen(listenfd, 2) < 0) {
        mx_printerr("uchat_server: couldn't listen for connections");
        exit(1);
    }
    cli_size = sizeof(cli);
    while (1) {
        if((newfd = accept(listenfd, (struct sockaddr*)&cli, (socklen_t *)&cli_size)) == -1) {
            mx_printerr("uchat_server: error accepting connection on a socket");
            exit(1);
        }
        mx_printint(newfd);
        printf("%s ", "\033[0;32mUser connected from ip:\033[0;32m");
        printf("%s\n", inet_ntoa(cli.sin_addr));
        if ((status = pthread_create(&thread, NULL, cycle, &newfd)) != 0) {
            mx_printerr("uchat_server: thread creating error");
            exit(1);
        }
        pthread_join(thread,NULL);


        close(newfd);
    }
}
