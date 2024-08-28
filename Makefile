NAME = cub3d

CC = cc

CFLAGS = -Wall -Werror -Wextra -Imlx -g -I./includes

MLXFLAGS = -L ./libs/minilibx-linux -lmlx -lXext -lX11

LIBS = -L ./libs/libft -lft

MAIN = main
INIT = init window minimap
PARSING = read_map
MOVEMENT = raycasting
UTILS = error check
# TOOLS = free parsing parsing2 type token fd expansions

SRCS = $(addsuffix .c, $(addprefix srcs/main/, $(MAIN))) \
	   $(addsuffix .c, $(addprefix srcs/init/, $(INIT))) \
	   $(addsuffix .c, $(addprefix srcs/parsing/, $(PARSING))) \
	   $(addsuffix .c, $(addprefix srcs/movement/, $(MOVEMENT))) \
	   $(addsuffix .c, $(addprefix srcs/utils/, $(UTILS))) \

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@echo "\n"
	@make -C ./libs/minilibx-linux
	@make -C ./libs/libft
	@echo "\033[0;32mCompiling cub3d..."
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS) $(MLXFLAGS)
	@echo "\n\033[0mDone!"

%.o: %.c
	@printf "\033[0;33mGenerating cub3d objects... %-33.33s\r" $@
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "\033[0;31mCleaning minilibx and libft..."
	@make clean -C ./libs/minilibx-linux
	@make clean -C ./libs/libft
	@echo "\nRemoving binaries..."
	@rm -f $(OBJS)
	@echo "\033[0m"

fclean: clean
	@echo "\nDeleting executable..."
	@rm -f $(NAME)
	@echo "\033[0m"

re: fclean all

.PHONY: all clean fclean re

