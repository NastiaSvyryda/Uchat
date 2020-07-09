NAME = uchat_server
NAME_ROOT = ../$(NAME)

CCFLG = clang -std=c11 $(addprefix -W, all extra error pedantic) -g

SRC_DIRS = src src/controllers src/controllers/auth src/controllers/messages \
src/json src/json/auth src/json/message src/migrations src/models src/system src/system/basic \
src/system/database_crud src/validations src/system/security
SRC_DIR = src

INC_DIR = inc
OBJ_DIR = obj

SRCS := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
INC = $(INC_DIR)/uchat_server.h
OBJS := $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:%.c=%.o)))

LIBMX_DIR = libmx
LIBMX = $(LIBMX_DIR)/libmx.a

GTK_FLAGS = `pkg-config --cflags  --libs gtk+-3.0` `pkg-config --libs json-c`
GTK_SORT_FLAGS = `pkg-config --cflags gtk+-3.0` `pkg-config --cflags json-c`

EMPTY =
SPACE = $(EMPTY) $(EMPTY)
VPATH := $(subst $(SPACE), :, $(SRC_DIRS))

all: install

install: $(NAME)

$(NAME): $(OBJS) $(LIBMX)
	@$(CCFLG) -lsqlite3 $(OBJS) $(GTK_FLAGS) -L$(LIBMX_DIR) -lmx -o $@ --debug
	@printf "\r\33[2K$@ \033[32;1mcreated\033[0m\n"

$(OBJS): obj/%.o: %.c $(INC) Makefile | $(OBJ_DIR)
	@$(CCFLG) -I$(INC_DIR) -Ilibmx/inc -c $(GTK_SORT_FLAGS) -o $@ $< --debug
	@printf "\r\33[2K$(NAME) \033[33;1mcompile \033[0m$(<:$(SRC_DIR)/%.c=%) "

$(OBJ_DIR):
	@mkdir -p $@
	@printf "\r\33[2K$@ \033[32;1mcreated\033[0m\n"

$(LIBMX):
	@make -sC $(LIBMX_DIR)

out: $(NAME_ROOT)

$(NAME_ROOT): $(NAME)
	@cp $(NAME) ..
	@printf "\r\33[2K$< \033[32;1mcopied to the root\033[0m\n"

clean:
	@make -sC $(LIBMX_DIR) $@
	@rm -rf $(OBJ_DIR)
	@printf "$(OBJ_DIR)\t   \033[31;1mdeleted\033[0m\n"

uninstall: clean
	@make -sC $(LIBMX_DIR) $@
	@rm -rf $(NAME)
	@printf "$(NAME) \033[31;1muninstalled\033[0m\n"

reinstall: uninstall install