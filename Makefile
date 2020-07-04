CL_NAME = uchat
SV_NAME = uchat_server

CL_DIR = client
SV_DIR = server

all: install

install: $(CL_NAME) $(SV_NAME)

$(CL_NAME):
	@make -C $(CL_DIR)
	@cp $(CL_DIR)/$(CL_NAME) .

$(SV_NAME):
	@make -C $(SV_DIR)
	@cp $(SV_DIR)/$(SV_NAME) .

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