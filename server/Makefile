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
	json/mx_str_to_file.c \
	json/mx_json_parse.c \
	json/mx_json_log_in.c \
	json/mx_json_register.c \
	json/mx_json_log_out.c \
    migrations/mx_migration.c \
    migrations/mx_migration_channels.c \
    migrations/mx_migration_delivery_user.c \
    migrations/mx_migration_messages.c \
    migrations/mx_migration_user_channel.c \
    migrations/mx_migration_users.c \
    models/mx_model_channel.c \
    models/mx_model_delivery_user.c \
    models/mx_model_message.c \
    models/mx_model_user.c \
    models/mx_model_user_channel.c \
    system/database_crud/mx_create_database.c \
    system/database_crud/mx_delete_database.c \
    system/database_crud/mx_read_database.c \
    system/database_crud/mx_update_database.c \
    validations/mx_valid_check_argc_error.c \
    validations/mx_valid_sqlite3_failed_data.c \
    validations/mx_valid_sqlite3_open_db.c \
    mx_config.c \
    main.c \
    mx_routes.c)

OBJ = \
	mx_str_to_file.c \
	mx_json_parse.c \
	mx_json_log_in.c \
	mx_json_register.c \
	mx_json_log_out.c \
    mx_migration.o \
    mx_migration_channels.o \
    mx_migration_delivery_user.o \
    mx_migration_messages.o \
    mx_migration_user_channel.o \
    mx_migration_users.o \
    mx_model_channel.o \
    mx_model_delivery_user.o \
    mx_model_message.o \
    mx_model_user.o \
    mx_model_user_channel.o \
    mx_create_database.o \
    mx_delete_database.o \
    mx_read_database.o \
    mx_update_database.o \
    mx_valid_check_argc_error.o \
    mx_valid_sqlite3_failed_data.o \
    mx_valid_sqlite3_open_db.o \
    mx_config.o \
    main.o \
    mx_routes.o

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