NAME = npuzzle
CC = g++
CFLAGS = -Wall -Wextra -Werror

INCLUDES = -I includes

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/Parser.cpp \
       $(SRC_DIR)/Heuristics.cpp \
       $(SRC_DIR)/Solver.cpp

OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)
	rm -f puzzle.txt
	rm -f solution.txt

re: fclean all

.PHONY: all clean fclean re