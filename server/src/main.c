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
    int *x_ptr = (int *)newfd;
    char recvBuff[1024];
//    mx_printint(*x_ptr);
//    mx_printstr("\nhallo world\n");
//    write(*x_ptr, "pussy", mx_strlen("pussy"));
    read(*x_ptr, recvBuff, sizeof(recvBuff));
//    char buff[1024];
    mx_printstr("\n");
    mx_printstr(recvBuff);
    mx_printstr("\n");
//    int i = 0;
//    void web_child(int);
//
//    pthread_detach(pthread_self());
//
////    web_child((int)arg);
//
////    while(i < 2) {
//        memset(buff, '\0', 1024);
////        mx_printstr("ARNI");
//        scanf("%s", buff);
//
//        write(*x_ptr, buff, mx_strlen(buff));
//        i++;
////    }
////        pthread_join(thread,NULL);
//    close(*x_ptr);
    return NULL;
}

int main(int argc, char **argv) {
    int fd[2];
    int i = 0;
    int listenfd = 0;
    int newfd = 0;
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
        } else {
            fd[i] = newfd;
            if ((status = pthread_create(&thread, NULL, cycle, &fd[i])) != 0) {
                mx_printerr("uchat_server: thread creating error");
                exit(1);
            }
        }
        printf("%s ", "\033[0;32mUser connected from ip:\033[0;32m");
        printf("%s\n", inet_ntoa(cli.sin_addr));
    }
}
