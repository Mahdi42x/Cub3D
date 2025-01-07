# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude

# Source files
SRC = main.c
OBJ = $(SRC:.c=.o)

# MiniLibX
MLX_DIR = include
MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

# Output binary
NAME = cub3D

# Rules
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(MLX_FLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
