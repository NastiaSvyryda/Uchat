APP_NAME = uchat

LIBMXF = libmx

LIB_NAME = libmx.a

INC = \
inc/ \
libmx/inc/

SRC_DIR = src

OBJ_DIR = obj

SRC = $(addprefix $(SRC_DIR)/,\
	main.c \
)

OBJ = main.o \

	
CC = clang

CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

all : install

install : libmx/libmx.a uchat

libmx/libmx.a:
	@make -C $(LIBMXF)

uchat : $(SRC) inc/uchat.h libmx/libmx.a
	@$(CC) $(CFLAGS) -c $(SRC) $(foreach d, $(INC), -I $d)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBMXF)/$(LIB_NAME) -o $(APP_NAME) -ltermcap
	@printf "\r\33[2Kuchat\t   \033[32;1mcreated\033[0m\n"
	@mkdir -p $(OBJ_DIR)
	@mv $(OBJ) $(OBJ_DIR)

uninstall : clean
	@make uninstall -C $(LIBMXF)
	@rm -rf $(APP_NAME)

clean :
	@make clean -C $(LIBMXF)
	@rm -rf $(OBJ_DIR)

reinstall : uninstall install