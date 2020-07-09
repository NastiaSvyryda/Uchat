#include "uchat_server.h"

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
    char name_server[100] = "";
    sprintf(name_server, "server_type_%d.json", type);
    char *str1 = mx_json_make_json(type, &json_data);
    struct json_object *jo = json_tokener_parse(str1 + 4);
    str1 = (char *)json_object_to_json_string_ext(jo, JSON_C_TO_STRING_PRETTY
        | JSON_C_TO_STRING_SPACED);
    mx_str_to_file(name_server, str1);
    char name[100] = "";
    sprintf(name, "client_type_%d.json", type);
    char *str = mx_file_to_str(name);
        if (!str)
        return;

    printf("client file str = \n%s\n", str);
    t_json_data *recieved_data = mx_json_parse(str);
    recieved_data ? printf("\033[32;1mOK\033[0m\n")
                  : printf("\033[31;1mNOT OK\033[0m\n\n");
    // printf("str form received data = \n%s\n----------\n\n", str1 + 4);

    if (recieved_data && recieved_data->message.text)
    free(recieved_data->message.text);
    free(recieved_data);
}

int main() {
    system("rm -rf server_type* test.jsonon");
    for (int i = 0; i < JS_NUM; i++) {
        fn(i);
    }
    // char *str = mx_file_to_str("./.vscode/settings.jsonon");
    // struct json_object *jo = json_tokener_parse(str);
    // str = (char *)json_object_to_json_string_ext(jo, JSON_C_TO_STRING_PRETTY
    //      );
    // mx_str_to_file("test.jsonon", str);
    // char *str = "{ \"type\": 346, \"status\": \"2006\", \"message_id\": 50, \"client2_id\": 2, \"token\": \"TOKEN!@#\" }";
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
