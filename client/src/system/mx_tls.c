#include "uchat_client.h"

int mx_open_connection(const char *hostname, int port) {
    int sd;
    struct hostent *host;
    struct sockaddr_in addr;

    if ( (host = gethostbyname(hostname)) == NULL ) {
        perror(hostname);
        abort();
    }
    if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        mx_printerr("uchat: couldn't create socket");
        exit(1);
    }
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if ((inet_pton(AF_INET, hostname, &addr.sin_addr)) <= 0) {
        mx_printerr("uchat: network adress isn't valid");
        exit(1);
    }
    if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 ) {
        close(sd);
        mx_printerr("uchat: connection failed\n");
        exit(1);
    }
    return sd;
}

SSL_CTX* mx_init_ctx(void) {
    const struct ssl_method_st *method;
    SSL_CTX *ctx;

    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    method = TLS_client_method();
    ctx = SSL_CTX_new(method);
    if ( ctx == NULL ) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}

void mx_show_certs(SSL* ssl) {
    X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl);
    if ( cert != NULL ) {
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        free(line);
        X509_free(cert);
    }
}
