#include "uchat_client.h"

    t_json_data json_data = {
        .type = type,
        .status = 200,
        .token = "TOKEN!@#",
        .pers_info = {
            .login = "xlogin",
            .password = "xpassword",
            .user_id = 1,
            .first_name = "John",
            .last_name = "Kras",
        },
        .message = {
            .text = "This is the text",
            .client1_id = 1,
            .client2_id = 2,
            .channel_id = 30,
            .message_id = 50,
            .delivery_time = 12345,
        }
    };
    char name[100] = "";
    sprintf(name, "client_type_%d.js", type);
    char *str = mx_json_make_json(type, json_data);
    mx_str_to_file(name, str + 4);
    char name_server[100] = "";
    sprintf(name_server, "server_type_%d.js", type);
    char *str1 = mx_file_to_str(name_server);
    printf("server file str = \n%s\n", str1);
    t_json_data *recieved_data = mx_json_parse(str1);
    recieved_data ? printf("OK\n") : printf("NOT OK\n");
    // printf("str form received data = \n%s\n----------\n\n", str1 + 4);

    if (recieved_data && recieved_data->message.text)
    free(recieved_data->message.text);
    free(recieved_data);
    free(str);
    free(str1);
}

int main() {
    for (int i = 0; i < JS_NUM; i++) {
        fn(i);
    }
    char *str = "{ \"type\": 346, \"status\": \"2006 sdf\", \"message_id\": 50, \"client2_id\": 2, \"token\": \"TOKEN!@#\" }";
    printf("extreme str = \n%s\n", str);
    t_json_data *json_data = mx_json_parse(str);
    // if (!json_data) {
    //     fprintf(stderr, "BAD JSON\n");
    //     return 0;
    // }
    char *str1 = mx_json_make_json(JS_MES_DEL_IN, json_data);
    printf("extreme str form received data = \n%s\n----------\n\n", str1 ? str1 + 4 : NULL);
    free(str1);
    system("leaks -q uchat");
    return 0;
}
