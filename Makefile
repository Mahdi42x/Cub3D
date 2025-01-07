# Output binary
NAME = cub3D

# Compiler and flags
CC = cc
CFLAGS = -Wall -Werror -Wextra -Imlx -Iincludes -g

MLXFLAGS = -L ./libs/mlx -lmlx -lXext -lX11 -lm
LIBS = ./libs/libft/libft.a

# Source files
SRC = main.c
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
