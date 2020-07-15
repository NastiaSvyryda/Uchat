#include "uchat_server.h"

void mx_logger(const char* tag, const char* message) {
    char *log;
    FILE *pFile;

    time_t now;
    time(&now);
    asprintf(&log, "%s [%s]: %s\n", ctime(&now), tag, message);
    pFile = fopen("log.txt", "a");
    if (pFile == NULL) {
        printf("Error logs\n");
        return;
    }
    fprintf(pFile, log, sizeof(log));
    fclose(pFile);
    mx_strdel(&log);
}
