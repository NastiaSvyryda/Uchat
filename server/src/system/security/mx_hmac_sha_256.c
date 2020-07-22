#include "uchat_server.h"

char *mx_hmac_sha_256(char *key, char *data) {
    unsigned char *result = NULL;
    unsigned int result_len = 0;
    result = HMAC(EVP_sha256(), (const void *)key, strlen(key),
                  (const unsigned char *)data, strlen(data), result,
                  &result_len);
    for (int i = 0; result[i] != '\0' ; ++i) {
        if (result[i] == '\'')
            result[i] = '.';
        if (result[i] == '`')
            result[i] = '.';
        if (result[i] == '"')
            result[i] = '.';
    }
    return (char *)result;
}
