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

int main() {
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;

    char sendBuff[1025];
    //time_t ticks;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);
    mx_printint(5);

    while(1) {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

        //ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%s\n", "hello");
        write(connfd, sendBuff, strlen(sendBuff));
        write(connfd, mx_itoa(connfd), mx_strlen(mx_itoa(connfd)));
        close(connfd);
        sleep(1);
    }
}
