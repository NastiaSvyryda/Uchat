#include "uchat_server.h"

void mx_migration(char *argv) {
    if (mx_strcmp(argv, "migration-up") == 0) {
        mx_migration_users(true);
        mx_migration_channels(true);
        mx_migration_messages(true);
        mx_migration_user_channel(true);
        mx_migration_delivery_user(true);
        exit(0);
    } else if (mx_strcmp(argv, "migration-ds") == 0) {
        mx_migration_users(false);
        mx_migration_channels(false);
        mx_migration_messages(false);
        mx_migration_user_channel(false);
        mx_migration_delivery_user(false);
        exit(0);
    }
}
