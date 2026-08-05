NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -MMD -MP -g
CFLAGS = -g #-fsanitize=address -pthread
BIN_DIR = src/.bin

CHECKERS = src/parser/checkers

PARSING_FILE = src/parser/parsing.c \
	$(CHECKERS)/is_number_checker.c \
	$(CHECKERS)/scheduler_checker.c \


SRC = $(PARSING_FILE) \
		src/main.c \
		src/routine/coder_routine/coder_routine.c \
		src/routine/monitoring_routine/monitoring_routine.c \
		src/monitor_init.c \
		src/coder_init.c \
		src/utils.c \
		src/dongle_init.c

OBJ = $(patsubst %.c, $(BIN_DIR)/%.o , $(SRC))
DEP = $(patsubst %.c, $(BIN_DIR)/%.d, $(SRC))

all : $(NAME)
$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ 

$(BIN_DIR)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean : 
	rm -rf $(BIN_DIR) .vscode/

fclean : clean
	rm -f $(NAME)

re : fclean
	$(MAKE) all

-include $(DEP)

.PHONY : all clean fclean re