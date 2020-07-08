#include "uchat_client.h"

static void fn(int type) {
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
    char *str = mx_json_make_json(type, &json_data);
    printf("made str = \n%s\n", str + 4);
    t_json_data *recieved_data = mx_json_parse(str + 4);
    char *str1 = mx_json_make_json(type, recieved_data);
    printf("str form received data = \n%s\n----------\n\n", str1 + 4);
}

int main() {
    for (int i = 0; i < JS_NUM; i++) {
        fn(i);
    }
    return 0;
}
