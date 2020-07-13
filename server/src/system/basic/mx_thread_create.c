#include "uchat_server.h"
void ShowCerts(SSL* ssl)
{
    X509 *cert;
    char *line;
    cert = SSL_get_peer_certificate(ssl); /* Get certificates (if available) */
    if ( cert != NULL )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);
        X509_free(cert);
    }
    else
        printf("No certificates.\n");
}
void *main_cycle(void *newfd) {
    t_json_data *json = NULL;
    t_clients *client = (t_clients *)newfd;
    t_clients *cur_client = client;
    int len = 0;
    char *json_str = NULL;
    if (SSL_accept(client->ssl) == -1)     /* do SSL-protocol accept */
        ERR_print_errors_fp(stderr);
    else {
        ShowCerts(client->ssl);
        while (true) {
            client = client->first;
            SSL_read(client->ssl, &len, 4);
            json_str = mx_strnew(len);
            SSL_read(client->ssl, json_str, len);
            mx_printstr(json_str);
            json = mx_json_parse(json_str);
            mx_routes(json, client, cur_client);
            if (json != NULL) {
                free(json);
                mx_strdel(&json_str);
            }
        }
    }
    return NULL;
}

void mx_thread_create(t_clients *client, struct sockaddr_in cli) {
    pthread_t thread = NULL;

    if ((pthread_create(&thread, NULL, main_cycle, client)) != 0) {
        mx_printerr("uchat_server: thread creating error");
        exit(1);
    }
    printf("%s ", "\033[0;32mUser connected from ip:\033[0;32m");
    printf("%s\n", inet_ntoa(cli.sin_addr));
}
