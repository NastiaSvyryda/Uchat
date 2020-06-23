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
#define MX_PORT 5000

//int main() {
//    int listenfd = 0, connfd = 0;
//    struct sockaddr_in serv_addr;
//
//    char sendBuff[1025];
//    //time_t ticks;
//
//    listenfd = socket(AF_INET, SOCK_STREAM, 0);
//    memset(&serv_addr, '0', sizeof(serv_addr));
//    memset(sendBuff, '0', sizeof(sendBuff));
//
//    serv_addr.sin_family = AF_INET;
//    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//    serv_addr.sin_port = htons(5000);
//
//    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
//
//    listen(listenfd, 10);
//    mx_printint(5);
//    char *mess;
//
//    while(1) {
//        mess = mx_strnew(1000);
//        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
//
//        //ticks = time(NULL);
//        scanf("%s", mess);
//        mx_printstr(mess);
//        write(connfd, mess, strlen(mess));
//        close(connfd);
//        sleep(1);
//        mx_strdel(&mess);
//    }
//}
int main(int argc, char **argv) {
    int listenfd = 0;
    int newfd = 0;
    int cli_size;
    struct sockaddr_in serv;
    struct sockaddr_in cli;

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
        if((newfd = accept(listenfd, (struct sockaddr*)&cli, (socklen_t *)&cli_size)
                ) == -1) {
            mx_printerr("uchat_server: error accepting connection on a socket");
            exit(1);
        }
        printf("%s ", "\033[0;32mUser connected from port:\033[0;32m");
        printf("%s\n", inet_ntoa(cli.sin_addr));
        write(newfd, "HALLO", mx_strlen("HALLO"));
        close(newfd);
//        mx_strdel(&str);
    }



}
