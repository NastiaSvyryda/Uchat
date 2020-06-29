// #include "inc/libmx.h"
#include "../inc/header.h"
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <locale.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <sys/xattr.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/wait.h>
#include <time.h>
#include <err.h>
#include <signal.h>
#include <stdarg.h>
// #include "inc/libmx.h"
#define KEY1 "peace-code"
#define KEY2 "death-ray-code"
                                                                                                     

/*
 * Reference counting example. The idea is to make a json_object res composed from
 * parts of obj1 and obj2.
 *
 * clang -Wall -g -I/usr/include/json-c/ -o json_refcount json_refcount.c -ljson-c
 */
#include <json.h>
#include <stdio.h>

/*
 * Just a utility function.
 */
void print_json_object(struct json_object *jobj, const char *msg) {
	printf("\n%s: \n", msg);
	printf("---\n%s\n---\n", json_object_to_json_string(jobj));
}

/*
 * In real life the operations done here are way more complicated, but it's
 * only an example.
 */
struct json_object * find_something(struct json_object *jobj, const char *key) {
	struct json_object *tmp;

	json_object_object_get_ex(jobj, key, &tmp);

	return tmp;
}

/*
 * Swap the first and second elements of the array received.
 * Again, more complicated in real life...
 */
void glitch_in_the_matrix(struct json_object *jobj) {
	struct json_object *pos1, *pos2;

	print_json_object(jobj, "Before the glitch in the matrix");

	pos1 = json_object_array_get_idx(jobj, 0);
	pos2 = json_object_array_get_idx(jobj, 1);

	// json_object_array_put_idx decrement the refcount if you replace an
	// element, but we don't whant that.
	json_object_array_put_idx(jobj, 0, json_object_get(pos2));
	json_object_array_put_idx(jobj, 1, json_object_get(pos1));

	print_json_object(jobj, "After the glitch in the matrix");
}

void fn(void) {
	// struct json_object *jo, *jo1, *jo2, *obj1, *jo3, *jo4, *obj2, *res, *sub_obj1, *sub_obj2, *tmp;
	struct json_object *jo;
	// struct array_list *joarr;
	// char *str = mx_file_to_str("./.vscode/c_cpp_properties.json");
	// char *str1;
	// printf("JSON:\n%s\n\n", str);
	t_personal_data p = {"xlogin", "xpass123", 1, "Steve", "Goy"};
	char *s = mx_json_register_response(&p, 200, "SDFSDF");
	jo = json_tokener_parse(s);
	// free(str);
	printf("jo from str:\n---\n%s\n---\n",
		   json_object_to_json_string_ext(jo, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));
	json_object_put(jo);
	free(s);
	return;


	// jo1 = find_something(jo, "configurations");
	// // joarr = json_object_get_array(jo1);
	// jo2 = json_object_array_get_idx(jo1, 0);
	// jo3 = find_something(jo2, "includePath");
	// // jo4 = json_object_new_object();
	// json_object_array_add(jo3, json_object_new_string("WORD"));
	// printf("jo NEW from str:\n---\n%s\n---\n",
	// 	   json_object_to_json_string_ext(jo, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));
	// // print_json_object(jo4, "jo4:\n in plaintext");



	// // Creation and print of obj1 and obj2
	// obj1 = json_object_new_object();
	// obj2 = json_object_new_object();
	// json_object_object_add(obj1, KEY1, json_object_new_int(1234));
	// json_object_object_add(obj2, KEY2, json_object_new_int(4321));
	// print_json_object(obj1, "obj1 in plaintext");
	// print_json_object(obj2, "obj2 in plaintext");

	// // Extraction of the interesting parts of obj1 and obj2
	// sub_obj1 = find_something(obj1, "peace-code");
	// sub_obj2 = find_something(obj2, "death-ray-code");
	// // print_json_object(sub_obj1, "sub_obj1 in plaintext");
	// // print_json_object(sub_obj2, "sub_obj2 in plaintext");
	// res = json_object_new_array();

	// // We are going to use those in res so we need to increment the
	// // refcount
	// json_object_array_add(res, json_object_get(sub_obj1));
	// json_object_array_add(res, json_object_get(sub_obj2));

	// glitch_in_the_matrix(res);

	// tmp = json_object_array_get_idx(res, 0);
	// printf("\n[>] Unlocking peace with code at index 0 of res: %d\n", json_object_get_int(tmp));

	// // We can safely put all of these objects
	// json_object_put(obj1);
	// json_object_put(obj2);
	// json_object_put(res);
	// json_object_put(jo);
	// // json_object_put(jo1);
	// // json_object_put(jo2);
	// // json_object_put(jo3);

	
}
int main() {
	// fn();
	// char *token = "SKDJFKSBDKF";
	t_message message = {"This is the text of the message", 1, 2, 10, 55, 10923};
	char *str = mx_json_message_edit_in_request(&message);
	printf(" str:\n---\n%s\n---\n", str);
	t_json_data *data = mx_json_parse(str);
	printf(MX_REQ_MES_EDIT_IN "\n---\n", data->type, data->data.message.message_id,
		   data->data.message.text, data->data.message.client1_id);
	char *s = mx_json_message_edit_in_request(&data->data.message);
	struct json_object *jo = json_tokener_parse(s);
	// free(str);
	printf("jo from str:\n---\n%s\n---\n",
		   json_object_to_json_string_ext(jo, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));
	json_object_put(jo);
	free(s);
	free(data->data.message.text);
	system("leaks -q json");
	return 0;
}
