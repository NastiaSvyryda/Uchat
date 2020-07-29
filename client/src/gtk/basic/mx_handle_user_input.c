#include "uchat_client.h"

char *mx_handle_user_input(const char *s) {
    char *oldW = "\\";
    char *newW = "\\\\\\\\";
    char *result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);

    for (i = 0; s[i] != '\0'; i++){
        if (strstr(&s[i], oldW) == &s[i]){
            cnt++;
            i += oldWlen - 1;
        }
    }
    result = mx_strnew(i + cnt * (newWlen - oldWlen) + 1);
    i = 0;
    while (*s){
        if (strstr(s, oldW) == s){
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }
    for (int y = 0; result[y]; ++y) {
        if (result[y] == '"' || result[y] == '\'')
            result[y] = '`';
    }
    result[i] = '\0';
    return result;
}

