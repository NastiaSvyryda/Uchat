APP_NAME = uchat

LIBMXF = libmx

LIB_NAME = libmx.a

INC = \
inc/ \
libmx/inc/

SRC_DIR = src

OBJ_DIR = obj

SRC = $(addprefix $(SRC_DIR)/,\
	json/mx_json_log_in.c \
	json/mx_json_log_out.c \
	json/mx_json_make_json.c \
	json/mx_json_message_delete_in.c \
	json/mx_json_message_delete_out.c \
	json/mx_json_message_edit_in.c \
	json/mx_json_message_edit_out.c \
	json/mx_json_message_in.c \
	json/mx_json_message_out.c \
	json/mx_json_parse.c \
	json/mx_json_register.c \
	json/mx_str_to_file.c \
	main.c)

OBJ = \
	mx_json_log_in.o \
	mx_json_log_out.o \
	mx_json_make_json.o \
	mx_json_message_delete_in.o \
	mx_json_message_delete_out.o \
	mx_json_message_edit_in.o \
	mx_json_message_edit_out.o \
	mx_json_message_in.o \
	mx_json_message_out.o \
	mx_json_parse.o \
	mx_json_register.o \
	mx_str_to_file.o \
	main.o

CC = clang

CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

GTK_FLAGS = `pkg-config --cflags  --libs gtk+-3.0` `pkg-config --libs json-c`
GTK_SORT_FLAGS = `pkg-config --cflags gtk+-3.0` `pkg-config --cflags json-c`

all : install

install : libmx/libmx.a uchat

libmx/libmx.a:
	@make -C $(LIBMXF)

uchat : $(SRC) inc/uchat_client.h libmx/libmx.a
	@$(CC) $(CFLAGS) -c $(GTK_SORT_FLAGS) $(SRC) $(foreach d, $(INC), -I $d)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBMXF)/$(LIB_NAME) $(GTK_FLAGS) -o $(APP_NAME) -ltermcap
	@printf "\r\33[2Kucaht \033[32;1mcreated\033[0m\n"
	@mkdir -p $(OBJ_DIR)
	@mv $(OBJ) $(OBJ_DIR)

uninstall : clean
	@make uninstall -C $(LIBMXF)
	@rm -rf $(APP_NAME)

clean :
	@make clean -C $(LIBMXF)
	@rm -rf $(OBJ_DIR)

reinstall : uninstall install