APP_NAME = uchat_server

LIBMXF = libmx

LIB_NAME = libmx.a

INC = \
../ \
inc/ \
libmx/inc/

SRC_DIR = src

OBJ_DIR = obj

SRC = $(addprefix $(SRC_DIR)/,\
    migrations/migration.c \
    migrations/migration_channels.c \
    migrations/migration_delivery_user.c \
    migrations/migration_messages.c \
    migrations/migration_user_channel.c \
    migrations/migration_users.c \
    models/model_channel.c \
    models/model_delivery_user.c \
    models/model_message.c \
    models/model_user.c \
    models/model_user_channel.c \
    system/database_crud/create_database.c \
    system/database_crud/delete_database.c \
    system/database_crud/read_database.c \
    system/database_crud/update_database.c \
    validations/valid_check_argc_error.c \
    validations/valid_sqlite3_failed_data.c \
    validations/valid_sqlite3_open_db.c \
    config.c \
    main.c \
    routes.c)

OBJ = \
    migration.o \
    migration_channels.o \
    migration_delivery_user.o \
    migration_messages.o \
    migration_user_channel.o \
    migration_users.o \
    model_channel.o \
    model_delivery_user.o \
    model_message.o \
    model_user.o \
    model_user_channel.o \
    create_database.o \
    delete_database.o \
    read_database.o \
    update_database.o \
    valid_check_argc_error.o \
    valid_sqlite3_failed_data.o \
    valid_sqlite3_open_db.o \
    config.o \
    main.o \
    routes.o

CC = clang

CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

all : install

install : ../libmx/libmx.a uchat_server

../libmx/libmx.a:
	@make -C $(LIBMXF)

uchat_server : $(SRC) inc/uchat_server.h ../libmx/libmx.a
	@$(CC) $(CFLAGS) -c $(SRC) $(foreach d, $(INC), -I $d)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBMXF)/$(LIB_NAME) -o $(APP_NAME) -ltermcap
	@printf "\r\33[2Kuchat_server \033[32;1mcreated\033[0m\n"
	@mkdir -p $(OBJ_DIR)
	@mv $(OBJ) $(OBJ_DIR)

uninstall : clean
	@make uninstall -C $(LIBMXF)
	@rm -rf $(APP_NAME)

clean :
	@make clean -C $(LIBMXF)
	@rm -rf $(OBJ_DIR)

reinstall : uninstall install