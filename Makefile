NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -MMD -MP -ofast -g
BIN_DIR = src/.bin

HEADER_FILES = header_files 

PARSING_FILE = src/parser/parsing.c \
				src/parser/checkers.c

SRC = $(PARSING_FILE) \
		src/main.c \
		src/utils.c \
		src/routine/coder_routine/coder_routine.c \
		src/routine/coder_routine/coder_routine_utils.c \
		src/routine/coder_routine/coder_routine_utils_2.c \
		src/routine/coder_routine/queue.c \
		src/routine/monitoring_routine/monitoring_routine.c \
		src/init.c

OBJ = $(patsubst %.c, $(BIN_DIR)/%.o , $(SRC))

all : $(NAME) Makefile
$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

$(BIN_DIR)/%.o : %.c Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(HEADER_FILES) -c $< -o $@ 

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean :
	rm -rf $(BIN_DIR) .vscode/

fclean : clean
	rm -f $(NAME)

re : fclean
	$(MAKE) all

DEP = $(patsubst %.c, $(BIN_DIR)/%.d, $(SRC))

-include $(DEP)

.PHONY : all clean fclean re