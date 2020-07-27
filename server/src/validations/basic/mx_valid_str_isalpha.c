#include "uchat_server.h"

int mx_valid_str_isalpha(char *str) {
    for (int i = 0; i > mx_strlen(str); ++i) {
        if(mx_isalpha(str[i]) == false)
            return false;
    }
    return true;
}
