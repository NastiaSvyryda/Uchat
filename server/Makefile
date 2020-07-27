NAME = uchat_server
NAME_ROOT = ../$(NAME)

CCFLG = clang -std=c11 $(addprefix -W, all extra error pedantic) -g

SRC_DIRS = src \
			src/controllers \
			src/controllers/auth \
			src/controllers/messages \
			src/controllers/user \
			src/json \
			src/json/user \
			src/json/auth \
			src/json/message \
			src/json/response/auth \
			src/json/response/messages \
			src/json/response/user \
			src/migrations \
			src/models \
			src/system \
			src/system/basic \
			src/system/database_crud \
			src/validations/basic \
			src/validations/auth \
			src/validations/database \
			src/system/security

SRC_DIR = src

INC_DIR = inc
OBJ_DIR = obj

SRCS := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
INC = $(INC_DIR)/uchat_server.h
OBJS := $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:%.c=%.o)))

LIBS_DIR = libs

LIBMX_DIR = $(LIBS_DIR)/libmx
LIBMX = $(LIBMX_DIR)/libmx.a

JSONC_DIR = $(LIBS_DIR)/json-c
JSONC = $(JSONC_DIR)/libjson-c.a

SSL_DIR = /usr/local/opt/openssl/lib/
SSL_INC = /usr/local/opt/openssl/include/
 

GTK_LINK_FLAGS = `pkg-config --cflags  --libs gtk+-3.0`
GTK_INC_FLAGS = `pkg-config --cflags gtk+-3.0`

LIBS_FLAGS := $(addprefix -L, $(SSL_DIR) \
	$(addprefix $(LIBS_DIR)/, json-c libmx)) \
	$(addprefix -l, ssl crypto sqlite3 json-c mx) $(GTK_LINK_FLAGS)
LIBS_INCS := $(addprefix -I, $(addprefix $(LIBS_DIR)/, json-c libmx/inc) \
	$(INC_DIR) $(SSL_INC)) $(GTK_INC_FLAGS)

LIBS = $(LIBMX) $(JSONC)

EMPTY =
SPACE = $(EMPTY) $(EMPTY)
VPATH := $(subst $(SPACE), :, $(SRC_DIRS))

all: install

install: $(NAME)

$(NAME): $(OBJS) $(LIBS)
	@$(CCFLG) $(LIBS_FLAGS) $(OBJS) -o $@ --debug
	@printf "\r\33[2K$@ \033[32;1mcreated\033[0m\n"

$(OBJS): obj/%.o: %.c $(INC) Makefile | $(OBJ_DIR)
	@$(CCFLG) $(LIBS_INCS) -c -o $@ $< --debug
	@printf "\r\33[2K$(NAME) \033[33;1mcompile \033[0m$(<:$(SRC_DIR)/%.c=%) "

$(OBJ_DIR):
	@mkdir -p $@
	@printf "\r\33[2K$@ \033[32;1mcreated\033[0m\n"

$(LIBMX):
	@make -sC $(LIBMX_DIR)

$(JSONC):
	@make -sC $(JSONC_DIR)

out: $(NAME_ROOT)

$(NAME_ROOT): $(NAME)
	@cp $(NAME) ..
	@printf "\r\33[2K$< \033[32;1mcopied to the root\033[0m\n"

clean:
	@make -sC $(LIBMX_DIR) $@
	@make -sC $(JSONC_DIR) $@
	@rm -rf $(OBJ_DIR)
	@printf "$(OBJ_DIR)\t   \033[31;1mdeleted\033[0m\n"

uninstall: clean
	@make -sC $(LIBMX_DIR) $@
	@make -sC $(JSONC_DIR) $@
	@rm -rf $(NAME)
	@printf "$(NAME) \033[31;1muninstalled\033[0m\n"

reinstall: uninstall install

db-up: install ./uchat_server migration-up
db-ds: install ./uchat_server migration-ds