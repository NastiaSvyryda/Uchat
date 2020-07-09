#include "uchat_server.h"

char *mx_create_token(int length) {
    static int mySeed = 25011984;
    char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#?!";
    size_t stringLen = strlen(string);
    char *randomString = NULL;

    srand(time(NULL) * length + ++mySeed);
    if (length < 1) {
        length = 1;
    }
    randomString = malloc(sizeof(char) * (length +1));
    short key = 0;
    for (int n = 0;n < length;n++) {
            key = rand() % stringLen;
            randomString[n] = string[key];
        }
    randomString[length] = '\0';
    return randomString;
}
