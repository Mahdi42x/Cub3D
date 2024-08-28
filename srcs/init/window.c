#include "../includes/cub3d.h"


int	close_window(t_game *game)
{   
    (void) game;
	ft_printf("Bye!\n");
	// free_exit(game);
	exit(EXIT_SUCCESS);
	return (0);
}

int	handle_key(int keycode, t_game *game)
{
	if (keycode == 53 || keycode == 27 || keycode == 9 || keycode == 65307)
		close_window(game);
	move_player(keycode, game);
	return (0);
}
