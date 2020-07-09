#include "uchat_client.h"

static void fn(int type) {
    t_json_data json_data = {
        .type = type,
        .status = 200,
        .token = "TOKEN",
        .pers_info = {
            .login = "xlogin",
            .password = "xpassword",
            .user_id = 1,
            .first_name = "John",
            .last_name = "Kras",
        },
        .message = {
            .text = "This is the text",
            .client1_id = 10,
            .client2_id = 20,
            .channel_id = 30,
            .message_id = 50,
            .delivery_time = 12345,
        },
    };
    char name[100] = "";
    sprintf(name, "./client/resources/client_type_%d.json", type);
    char *str = mx_json_make_json(type, &json_data);
    struct json_object *jo = json_tokener_parse(str + 4);
    // printf("---------\n----------\n%s\n---------\n---------\n",
    // json_object_to_json_string_ext(jo, JSON_C_TO_STRING_PRETTY
    //     | JSON_C_TO_STRING_SPACED));
    str = (char *)json_object_to_json_string_ext(jo, JSON_C_TO_STRING_PRETTY
        | JSON_C_TO_STRING_SPACED);
    mx_str_to_file(name, str);
    char name_server[100] = "";
    sprintf(name_server, "./server/resources/server_type_%d.json", type);
    char *str1 = mx_file_to_str(name_server);
    if (!str1)
        return;
    printf("server file str = \n%s\n", str1);
    t_json_data *recieved_data = mx_json_parse(str1);
    recieved_data ? printf("\033[32;1mOK\033[0m\n")
                  : printf("\033[31;1mNOT OK\033[0m\n\n");
    // printf("str form received data = \n%s\n----------\n\n", str1 + 4);

    if (recieved_data && recieved_data->message.text)
    free(recieved_data->message.text);
    free(recieved_data);
    // free(str);
    // free(str1);
}

int main() {
    system("rm -rf client_type* client/resources/client_type*");
    for (int i = 0; i < JS_NUM; i++) {
        fn(i);
    }
    // char *str = "{ \"type\": 346, \"status\": \"2006 sdf\", \"message_id\": 50, \"client2_id\": 2, \"token\": \"TOKEN!@#\" }";
    // printf("extreme str = \n%s\n", str);
    // t_json_data *json_data = mx_json_parse(str);
    // if (!json_data) {
    //     fprintf(stderr, "BAD JSON\n");
    //     return 0;
    // }
    // char *str1 = mx_json_make_json(JS_MES_DEL_IN, json_data);
    // printf("extreme str form received data = \n%s\n----------\n\n", str1 ? str1 + 4 : NULL);
    // free(str1);
    // system("leaks -q uchat");
    return 0;
}
