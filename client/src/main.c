#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include "../../libmx/inc/libmx.h"
//#include <gtk/gtk.h>
#include <pthread.h>

void *input(void *sock) {
    int *sockfd = (int *) sock;
    char recvBuff[1024];
    char name[100];
    while(1) {
        mx_printstr("NAME, WHO YOU WANNA SEND THE MESSAGE: ");
        scanf("%s", name);
        write(*sockfd, name, mx_strlen(name));
        mx_printstr("YOUR MESSAGE: ");
        scanf("%s", recvBuff);
        write(*sockfd, recvBuff, mx_strlen(recvBuff));
        memset(recvBuff, '\0', 1024);
        memset(name, '\0', 100);
    }
    return NULL;
}

int main(int argc, char **argv) {
    int sockfd = 0;
    struct sockaddr_in serv;
    //char recvBuff[1024];
    char buf[1024];
    pthread_t thread = NULL;
    char name[100];

    if (argc != 3) {
        mx_printerr("uchat_server: error args\n");
        mx_printerr("example: ./uchat ip port\n");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        mx_printerr("uchat: couldn't create socket");
        exit(1);
    }

    serv.sin_family = AF_INET;
    serv.sin_port = htons(atoi(argv[2]));

    if ((inet_pton(AF_INET, argv[1], &serv.sin_addr)) <= 0) {
        mx_printerr("uchat: network adress isn't valid");
        exit(1);
    }
    if ((connect(sockfd, (struct sockaddr *)&serv, sizeof(serv))) < 0) {
        mx_printerr("uchat: connection failed\n");
        exit(1);
    }
    memset(name, '\0', 100);
    mx_printstr("YOUR NAME: ");
    scanf("%s", name);
    write(sockfd, name, mx_strlen(name));
    if (pthread_create(&thread, NULL, input, &sockfd) != 0) {
        mx_printerr("uchat_server: thread creating error");
        exit(1);
    }
    while (1) {
//        if (read(sockfd, recvBuff, sizeof(recvBuff)) == -1) {
//            mx_printerr("uchat: error read");
//            exit(1);
//        }
        recv(sockfd, buf, 1024, 0);
        mx_printstr("\033[2K");
        mx_printstr("\033[G");
        mx_printstr("Message recived: ");
        mx_printstr(buf);
        mx_printstr("\nNAME, WHO YOU WANNA SEND THE MESSAGE: ");
        memset(buf, '\0', 1024);
//        pthread_join(thread, NULL);
//        pthread_cancel(thread);

    }
}
