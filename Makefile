NAME = codexion
CC = gcc
CFLAGS = -Wall -Wextra -Werror -MMD -MP -g
CFLAGS = -g #-fsanitize=address -pthread
BIN_DIR = src/.bin

HEADER_FILES = header_files 

CHECKERS = src/parser/checkers

PARSING_FILE = src/parser/parsing.c \
	$(CHECKERS)/is_number_checker.c \
	$(CHECKERS)/scheduler_checker.c \

SRC = $(PARSING_FILE) \
		src/main.c \
		src/utils.c \
		src/routine/coder_routine/coder_routine.c \
		src/routine/coder_routine/coder_routine_utils.c \
		src/routine/coder_routine/scheduler/queue.c \
		src/routine/coder_routine/scheduler/edf.c \
		src/routine/coder_routine/scheduler/fifo.c \
		src/routine/monitoring_routine/monitoring_routine.c \
		src/init_files/monitor_init.c \
		src/init_files/coder_init.c \
		src/init_files/dongle_init.c 

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