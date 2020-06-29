#include "uchat_server.h"

void migration(char *argv) {
    if (strcmp(argv, "migration up") == 0) {
        migration_users(0);
        migration_channels("up");
        migration_messages("up");
        migration_user_channel("up");
        migration_delivery_user(true);
    } else if (strcmp(argv, "migration destroy") == 0) {
        migration_users(false);
        migration_channels(false);
        migration_messages(false);
        migration_user_channel(false);
        migration_delivery_user(false);
    }
}
