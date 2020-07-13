#include "uchat_server.h"

SSL_CTX* mx_init_server_ctx(void) {
    const struct ssl_method_st *method;
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();  /* load & register all cryptos, etc. */
    SSL_load_error_strings();   /* load all error messages */
    method = TLS_server_method();  /* create new server-method instance */
    ctx = SSL_CTX_new(method);   /* create new context from method */
    if (ctx == NULL ) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}

void mx_load_certificates(SSL_CTX* ctx, char* CertFile, char* KeyFile) {
    /* set the local certificate from CertFile */
    if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 ) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* set the private key from KeyFile (may be the same as CertFile) */
    if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 ) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* verify private key */
    if ( !SSL_CTX_check_private_key(ctx) ) {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }
}
