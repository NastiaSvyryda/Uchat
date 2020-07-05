#include "uchat_client.h"

void mx_str_to_file(const char *filepath, const char *data) {
    FILE *fp = fopen(filepath, "ab");

    if (fp != NULL) {
        fputs(data, fp);
        fclose(fp);
    }
}
