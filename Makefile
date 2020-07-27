CL_NAME = uchat
SV_NAME = uchat_server

CL_DIR = client
SV_DIR = server

all: install

install: dep

dep:
	@git submodule init
	@git submodule update
	@make out -C $(CL_DIR)
	@make out -C $(SV_DIR)

clean:
	@make clean -C $(CL_DIR)
	@make clean -C $(SV_DIR)

uninstall: clean
	@make uninstall -C $(CL_DIR)
	@make uninstall -C $(SV_DIR)
	@rm -rf $(CL_NAME)
	@rm -rf $(SV_NAME)
	@printf "$(CL_NAME) \033[31;1muninstalled\033[0m\n"
	@printf "$(SV_NAME) \033[31;1muninstalled\033[0m\n"

reinstall: uninstall install

db-up: install ./uchat_server  migration-up
db-ds: install ./uchat_server migration-ds