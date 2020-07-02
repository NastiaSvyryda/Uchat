NAME = uchat_server

CCFLG = clang -std=c11 $(addprefix -W, all extra error pedantic) -g

SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj

INC = $(INC_DIR)/server.h
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:%.c=%.o)))

LIBMX_DIR = ../libmx
LIBMX = $(LIBMX_DIR)/libmx.a

GTK_FLAGS = `pkg-config --cflags  --libs gtk+-3.0` `pkg-config --libs json-c`
GTK_SORT_FLAGS = `pkg-config --cflags gtk+-3.0` `pkg-config --cflags json-c`


all: install

install: $(LIBMX) $(NAME)

$(NAME): $(OBJS)
	@$(CCFLG) $(OBJS) $(GTK_FLAGS) -L$(LIBMX_DIR) -lmx -o $@
	@printf "\r\33[2K$@ \033[32;1mcreated\033[0m\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC)
	@$(CCFLG) -c $(GTK_SORT_FLAGS) $(SRCS) -I$(INC)
	@printf "\r\33[2K$(NAME) \033[33;1mcompile \033[0m$(<:$(SRC_DIR)/%.c=%) "
	@mv *.o $(OBJ_DIR)

$(OBJS): | $(OBJ_DIR)

$(OBJ_DIR):
	@mkdir -p $@
	@printf "\r\33[2K$@ \033[32;1mcreated\033[0m\n"

$(LIBMX):
	@make -sC $(LIBMX_DIR)

clean:
	@make -sC $(LIBMX_DIR) $@
	@rm -rf $(OBJ_DIR)
	@printf "$(OBJ_DIR)\t   \033[31;1mdeleted\033[0m\n"

uninstall: clean
	@make -sC $(LIBMX_DIR) $@
	@rm -rf $(NAME)
	@printf "$(NAME) \033[31;1muninstalled\033[0m\n"

reinstall: uninstall install