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
#include "../../inc/header.h"

void *input(void *sock) {
    int *sockfd = (int *) sock;
    char recvBuff[1024];
    char name[100];
    char *json_str = NULL;
    t_json_data *json = malloc(sizeof(t_json_data));

    json->message.client1_id = 1;
    json->message.client2_id = 2;
    json->message.text = mx_strdup("hallo");
//        while(1) {
        json_str = mx_json_message_out_request(json);
        mx_printint(mx_strlen(json_str));
        write(*sockfd, json_str, mx_strlen(json_str));
        memset(recvBuff, '\0', 1024);
        memset(name, '\0', 100);
//    }

    return NULL;
}

int main(int argc, char **argv) {
    int sockfd = 0;
    struct sockaddr_in serv;
    char buf[1024];
    pthread_t thread = NULL;
    char name[100];
//    t_json_data *json = NULL;

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
        recv(sockfd, buf, 1024, 0);
//        json = mx_json_parse(buf);

        mx_printstr("Message recived: ");
        mx_printstr(buf);
//        mx_printstr(json->data.message.text);
        mx_printstr("\n from: ");
//        mx_printint(json->data.message.client1_id);
        memset(buf, '\0', 1024);
//        mx_strdel(&json->data.message.text);
//        free(json);
    }
}
