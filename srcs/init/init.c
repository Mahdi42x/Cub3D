#include "/home/mahdi/Schreibtisch/Cub3D/includes/cub3d.h"

void   init_struct(t_game *game, char *map)
{
    (void)game;
    (void)map;
    //INITIALIZE GAME STRUCT
}

void    init_game(t_game *game,  char *map)
{
    (void)map;
    
    game->mlx_connection = mlx_init();
    game->mlx_win = mlx_new_window(game->mlx_connection,
            10 * 64, 10 * 64, "cub3d");
    
    mlx_hook(game->mlx_win, 2, 1L << 0, handle_key, game);
    mlx_hook(game->mlx_win, 17, 0, close_window, game);
    mlx_loop(game->mlx_connection);
}