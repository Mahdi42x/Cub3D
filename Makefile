# Output binary
NAME = cub3D

# Compiler and flags
CC = cc
CFLAGS = -Wall -Werror -Wextra -Imlx -Iincludes -g

MLXFLAGS = -L ./libs/mlx -lmlx -lXext -lX11 -lm
LIBS = ./libs/libft/libft.a

# Source files
MAIN = main
INPUT = input
# PARSING = line line2 line3 tokens tokens2 expansions
# ENV = env shlvl get_env sort_env
# EXEC = bin builtin exec bin_help
# BUILTINS = env export export2 exit cd echo pwd unset
# TOOLS = free parsing parsing2 type token fd expansions

SRC = $(addsuffix .c, $(addprefix srcs/main/, $(MAIN))) \
	$(addsuffix .c, $(addprefix srcs/input/, $(INPUT))) \
	#   $(addsuffix .c, $(addprefix srcs/parsing/, $(PARSING))) \
	#   $(addsuffix .c, $(addprefix srcs/env/, $(ENV))) \
	#   $(addsuffix .c, $(addprefix srcs/exec/, $(EXEC))) \
	#   $(addsuffix .c, $(addprefix srcs/builtins/, $(BUILTINS))) \
	#   $(addsuffix .c, $(addprefix srcs/tools/, $(TOOLS))) \

OBJ = $(SRC:.c=.o)

# Rules
all: $(NAME)

$(NAME): $(OBJ)
	@echo "\n"
	@make -C ./libs/mlx
	@make -C ./libs/libft
	@echo "\033[0;32mCompiling CUB3D..."
	@$(CC) $(CFLAGS) $(OBJ) $(LIBS) $(MLXFLAGS) -o $(NAME)
	@echo "\n\033[0mDone !"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make clean -C ./libs/mlx
	make clean -C ./libs/libft
	rm -rf $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re
