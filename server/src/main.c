#include "uchat_server.h"

int main(int argc, char **argv) {
    migration(argv[1]);
    valid_check_argc_error(argc);
    return 0;
}
