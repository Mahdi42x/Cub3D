#include "../includes/cub3d.h"

int main(int argc, char **argv)
{
	t_game *game;

	game = malloc(sizeof(t_game));
	if (!game)
		err("Error: Memory allocation failed\n");
	if (argc == 2 && check_format(argv[1], ".cub"))
	{
		read_map(game, argv[1]);
		init_game(game, argv[1]);
	}
	else
			err("Error : Invalid arguments\n");
	free(game);
	return (0);
}
