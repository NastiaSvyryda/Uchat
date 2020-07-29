#include "uchat_client.h"

void mx_SSL_read_to_str(SSL *ssl, char *buf, int len) {
    int total = 0;
    int read_bytes = SSL_read(ssl, buf, BUFSIZ);

    for (total = read_bytes; total < len;
        read_bytes = SSL_read(ssl, buf, BUFSIZ)) {
        buf += read_bytes;
        total += read_bytes;
    }
}
