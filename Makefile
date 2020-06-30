CL_NAME = uchat
SV_NAME = uchat_server

CCFLG = clang -std=c11 $(addprefix -W, all extra error pedantic) -g

CL_SRC_DIR = client/src
SV_SRC_DIR = server/src

CL_INC_DIR = client/inc
SV_INC_DIR = server/inc

LIBMX_DIR = libmx
LIBMX = $(LIBMX_DIR)/libmx.a

CL_OBJ_DIR = client/obj
SV_OBJ_DIR = server/obj

GTK_FLAGS = `pkg-config --cflags  --libs gtk+-3.0` `pkg-config --libs json-c`
GTK_SORT_FLAGS = `pkg-config --cflags gtk+-3.0` `pkg-config --cflags json-c`

#CL_INC = $(CL_INC_DIR)/client.h
#SV_INC = $(SV_INC_DIR)/server.h
INC = inc/header.h
CL_SRC = main.c \
         mx_json_log_in.c \
         mx_json_log_out.c\
   		 mx_json_message_delete_in.c\
   		 mx_json_message_delete_out.c\
         mx_json_message_edit_in.c\
         mx_json_message_edit_out.c\
         mx_json_message_in.c\
         mx_json_message_out.c\
         mx_json_parse.c\
         mx_json_register.c\
         mx_json_type.c\
         mx_str_to_file.c

SV_SRC = main.c \
		 mx_json_log_in.c \
		 mx_json_log_out.c\
		 mx_json_message_delete_in.c\
		 mx_json_message_delete_out.c\
		 mx_json_message_edit_in.c\
		 mx_json_message_edit_out.c\
		 mx_json_message_in.c\
		 mx_json_message_out.c\
		 mx_json_parse.c\
		 mx_json_register.c\
		 mx_json_type.c\
		 mx_str_to_file.c

CL_SRCS = $(addprefix $(CL_SRC_DIR)/, $(CL_SRC))
CL_OBJS = $(addprefix $(CL_OBJ_DIR)/, $(CL_SRC:%.c=%.o))

SV_SRCS = $(addprefix $(SV_SRC_DIR)/, $(SV_SRC))
SV_OBJS = $(addprefix $(SV_OBJ_DIR)/, $(SV_SRC:%.c=%.o))

all: install

install: install_client install_server

install_client: $(LIBMX) $(CL_NAME)

$(CL_NAME): $(CL_OBJS)
	@$(CCFLG) $(CL_OBJS) $(GTK_FLAGS) -L$(LIBMX_DIR) -lmx -o $@
	@printf "\r\33[2K$@ \033[32;1mcreated\033[0m\n"

$(CL_OBJ_DIR)/%.o: $(CL_SRC_DIR)/%.c $(CL_INC)
	@$(CCFLG) -c $(GTK_SORT_FLAGS) $(CL_SRCS) -I$(INC)
	@printf "\r\33[2K$(CL_NAME) \033[33;1mcompile \033[0m$(<:$(CL_SRC_DIR)/%.c=%)"
	@mv $(CL_SRC:%.c=%.o) $(CL_OBJ_DIR)
    
$(CL_OBJS): | $(CL_OBJ_DIR)

$(CL_OBJ_DIR):
	@mkdir -p $@
	@printf "\r\33[2K$@ \033[32;1mcreated\033[0m\n"

install_server: $(LIBMX) $(SV_NAME)

$(SV_NAME): $(SV_OBJS)
	@$(CCFLG) $(SV_OBJS) $(GTK_FLAGS) -L$(LIBMX_DIR) -lmx -o $@
	@printf "\r\33[2K$@ \033[32;1mcreated\033[0m\n"

$(SV_OBJ_DIR)/%.o: $(SV_SRC_DIR)/%.c $(SV_INC)
	@$(CCFLG) -c $(GTK_SORT_FLAGS) $(SV_SRCS) -I$(INC)
	@printf "\r\33[2K$(SV_NAME) \033[33;1mcompile \033[0m$(<:$(SV_SRC_DIR)/%.c=%) "
	@mv $(SV_SRC:%.c=%.o) $(SV_OBJ_DIR)

$(SV_OBJS): | $(SV_OBJ_DIR)

$(SV_OBJ_DIR):
	@mkdir -p $@
	@printf "\r\33[2K$@ \033[32;1mcreated\033[0m\n"

$(LIBMX):
	@make -sC $(LIBMX_DIR)

clean:
	@make -sC $(LIBMX_DIR) $@
	@rm -rf $(CL_OBJ_DIR)
	@rm -rf $(SV_OBJ_DIR)
	@printf "$(CL_OBJ_DIR)\t   \033[31;1mdeleted\033[0m\n"
	@printf "$(SV_OBJ_DIR)\t   \033[31;1mdeleted\033[0m\n"

uninstall: clean
	@make -sC $(LIBMX_DIR) $@
	@rm -rf $(CL_NAME)
	@rm -rf $(SV_NAME)
	@printf "$(CL_NAME) \033[31;1muninstalled\033[0m\n"
	@printf "$(SV_NAME) \033[31;1muninstalled\033[0m\n"

reinstall: uninstall install