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
#include <gtk/gtk.h>

int main(int argc, char **argv) {
    int sockfd = 0;
    struct sockaddr_in serv;
    char recvBuff[1024];

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
    while (1) {
        if (read(sockfd, recvBuff, sizeof(recvBuff)) == -1) {
            mx_printerr("uchat: error read");
            exit(1);
        }
        if (mx_strcmp(recvBuff, "exit") == 0) {
            mx_printstr("mission completed");
            break;
        }
        mx_printstr(recvBuff);
        memset(recvBuff, '\0', 1024);

    }
}
