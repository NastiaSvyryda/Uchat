#include "uchat_client.h"

void *input(void *sock) {
    int *sockfd = (int *) sock;
    char recvBuff[1024];
    char name[100];
    char *json_str = NULL;
    t_json_data *json = malloc(sizeof(t_json_data));
    enum e_json_types type;

    type = JS_MES_OUT;
    json->message.client1_id = 1;
    json->message.client2_id = 2;
    json->message.text = mx_strdup("hallo");
//        while(1) {
    json_str = mx_json_make_json(type, json);
    mx_printint(mx_strlen(json_str));
    write(*sockfd, json_str, mx_strlen(json_str));
    memset(recvBuff, '\0', 1024);
    memset(name, '\0', 100);
//    }

    return NULL;
}

int main() {
	t_json_data jsd = {.message = {"This is the text of the message", 1, 2, 10, 55, 10923}, .type = JS_MES_OUT};
	char *str = mx_json_make_json(JS_MES_OUT, &jsd);
	printf(" str:\n---\n%s\n---\n", str + 4);
	t_json_data *data = mx_json_parse(str + 4);
	printf(MX_REQ_MES_OUT "\n---\n", data->type, data->message.client1_id, data->message.client2_id,
		   data->message.text);
	char *s = mx_json_make_json(JS_MES_OUT, data);
	struct json_object *jo = json_tokener_parse(s + 4);
	// free(str);
	printf("jo from str:\n---\n%s\n---\n",
		   json_object_to_json_string_ext(jo, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));
	json_object_put(jo);
	free(s);
	free(data->message.text);
	system("leaks -q uchat");
	return 0;
}
