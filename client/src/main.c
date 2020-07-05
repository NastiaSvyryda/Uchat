#include "uchat_client.h"

void *input(void *sock) {
int *sockfd = (int *) sock;
char *json_str = NULL;
t_json_data *json = malloc(sizeof(t_json_data));
enum e_json_types type;

type = JS_REG;
strcpy(json->pers_info.login, "spleshkov");
strcpy(json->pers_info.password, "qwerty");
strcpy(json->pers_info.first_name, "Viacheslav");
strcpy(json->pers_info.last_name, "Pleshkov");
//        while(1) {
json_str = mx_json_make_json(type, json);
mx_printint(mx_strlen(json_str));
write(*sockfd, json_str, mx_strlen(json_str));
//    }

return NULL;
}

int main(int argc, char **argv) {
int sockfd = 0;
struct sockaddr_in serv;
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
//    memset(name, '\0', 100);
//    mx_printstr("YOUR NAME: ");
//    scanf("%s", name);
write(sockfd, name, mx_strlen(name));
if (pthread_create(&thread, NULL, input, &sockfd) != 0) {
mx_printerr("uchat_server: thread creating error");
exit(1);
}
//    while (1) {
//        recv(sockfd, buf, 1024, 0);
////        json = mx_json_parse(buf);
//
//        mx_printstr("Message recived: ");
//        mx_printstr(buf);
////        mx_printstr(json->data.message.text);
//        mx_printstr("\n from: ");
////        mx_printint(json->data.message.client1_id);
//        memset(buf, '\0', 1024);
////        mx_strdel(&json->data.message.text);
////        free(json);
//    }
}

