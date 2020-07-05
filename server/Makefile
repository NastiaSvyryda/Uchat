APP_NAME = uchat_server

LIBMXF = libmx

LIB_NAME = libmx.a

INC = \
inc/ \
libmx/inc/

SRC_DIR = src

OBJ_DIR = obj

SRC = $(addprefix $(SRC_DIR)/,\
	controllers/auth/mx_controller_log_out.c \
	controllers/auth/mx_controller_login.c \
	controllers/auth/mx_controller_register.c \
	controllers/messages/mx_controller_message.c \
	json/mx_json_make_json.c \
	json/mx_json_parse.c \
	json/mx_str_to_file.c \
	json/auth/mx_json_log_in.c \
	json/auth/mx_json_log_out.c \
	json/auth/mx_json_register.c \
	json/message/mx_json_message_delete_in.c \
	json/message/mx_json_message_delete_out.c \
	json/message/mx_json_message_edit_in.c \
	json/message/mx_json_message_edit_out.c \
	json/message/mx_json_message_in.c \
	json/message/mx_json_message_out.c \
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
	system/basic/mx_accept_connections.c \
	system/basic/mx_conn_list_sock.c \
	system/basic/mx_thread_create.c \
	system/database_crud/mx_create_database.c \
	system/database_crud/mx_delete_database.c \
	system/database_crud/mx_read_database.c \
	system/database_crud/mx_update_database.c \
	validations/mx_valid_check_argc_error.c \
	validations/mx_valid_sqlite3_failed_data.c \
	validations/mx_valid_sqlite3_open_db.c \
	mx_config.c \
	mx_routes.c \
	main.c)

OBJ = \
	mx_controller_log_out.o \
	mx_controller_login.o \
	mx_controller_register.o \
	mx_controller_message.o \
	mx_json_make_json.o \
	mx_json_parse.o \
	mx_str_to_file.o \
	mx_json_log_in.o \
	mx_json_log_out.o \
	mx_json_register.o \
	mx_json_message_delete_in.o \
	mx_json_message_delete_out.o \
	mx_json_message_edit_in.o \
	mx_json_message_edit_out.o \
	mx_json_message_in.o \
	mx_json_message_out.o \
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
	mx_accept_connections.o \
	mx_conn_list_sock.o \
	mx_thread_create.o \
	mx_create_database.o \
	mx_delete_database.o \
	mx_read_database.o \
	mx_update_database.o \
	mx_valid_check_argc_error.o \
	mx_valid_sqlite3_failed_data.o \
	mx_valid_sqlite3_open_db.o \
	mx_config.o \
	mx_routes.o \
	main.o
CC = clang

GTK_FLAGS = `pkg-config --cflags  --libs gtk+-3.0` `pkg-config --libs json-c`
GTK_SORT_FLAGS = `pkg-config --cflags gtk+-3.0` `pkg-config --cflags json-c`

CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

all : install

install : libmx/libmx.a uchat_server

libmx/libmx.a:
	@make -C $(LIBMXF)

uchat_server : $(SRC) inc/uchat_server.h libmx/libmx.a
	@$(CC) $(CFLAGS) -c $(GTK_SORT_FLAGS) $(SRC) $(foreach d, $(INC), -I $d)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBMXF)/$(LIB_NAME) $(GTK_FLAGS) -lsqlite3 -o $(APP_NAME) -ltermcap
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