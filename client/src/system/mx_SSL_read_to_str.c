#include "uchat_client.h"

void mx_SSL_read_to_str(SSL *ssl, char *buf, int len) {
    int total = 0;
    int read_bytes = SSL_read(ssl, buf, BUFSIZ);
    // printf("read_bytes = %d\n, total = %d\n, len = %d\n", read_bytes, total, len);

    for (total = read_bytes; total < len;
        read_bytes = SSL_read(ssl, buf, BUFSIZ)) {
        buf += read_bytes;
        total += read_bytes;
        // printf("read_bytes = %d\n, total = %d\n, len = %d\n", read_bytes, total, len);
    }
}
