#include "../includes/cub3d.h"

void   init_struct(t_game *game, char *map)
{
    (void)game;
    (void)map;
    //INITIALIZE GAME STRUCT

    game->win_x = SIZE_X;
	game->win_y = SIZE_Y;
}

void    init_game(t_game *game,  char *map)
{
    (void)map;
    
    game->mlx_connection = mlx_init();
    read_map(game, map);
    game->mlx_win = mlx_new_window(game->mlx_connection,
            game->win_x, game->win_y, "CUB3D");
    raycasting(game);
    // minimap(game);

    mlx_hook(game->mlx_win, 2, 1L << 0, handle_key, game);
    mlx_hook(game->mlx_win, 17, 0, close_window, game);
    mlx_loop(game->mlx_connection);
}