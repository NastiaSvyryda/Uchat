#include "server.h"

void migration(char *argv) {
    if (strcmp(argv, "migration") == 0) {
        migration_users();
        migration_channels();
        migration_messages();
        migration_user_channel();
        migration_delivery_user();
    }
}