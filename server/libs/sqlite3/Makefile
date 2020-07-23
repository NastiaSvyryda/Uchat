NAME = libsqlite3.a

SRC := $(wildcard *.c)
OBJ = $(addprefix obj/, $(notdir $(SRC:%.c=%.o)))

all: $(NAME)

install: $(NAME)

$(NAME): $(OBJ)
	@make clean
	@ar -rcs $(NAME) $(OBJ)
	@printf "\x1b[32;1m$@ created\x1b[0m\n"

$(OBJ): | obj

obj:
	@mkdir obj

obj/%.o: %.c
	@clang $(FLAGS) -c $< -o $@
	@printf "\x1b[32mcompiled: \x1b[0m[sqlite3/$<]\n"

uninstall:
	@make clean
	@rm -rf ./obj
	@rm -rf $(NAME)

clean:
	@rm -rf *.o

reinstall: uninstall install
