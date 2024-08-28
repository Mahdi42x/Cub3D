#include "../includes/cub3d.h"

void init_player_direction(t_game *game) {
    if (game->player_dir == NORTH) {
        game->dir_x = 0;
        game->dir_y = -1;
        game->plane_x = 0.66;
        game->plane_y = 0;
    } else if (game->player_dir == SOUTH) {
        game->dir_x = 0;
        game->dir_y = 1;
        game->plane_x = -0.66;
        game->plane_y = 0;
    } else if (game->player_dir == EAST) {
        game->dir_x = 1;
        game->dir_y = 0;
        game->plane_x = 0;
        game->plane_y = 0.66;
    } else if (game->player_dir == WEST) {
        game->dir_x = -1;
        game->dir_y = 0;
        game->plane_x = 0;
        game->plane_y = -0.66;
    } else {
        // Fehlerbehandlung, wenn eine unbekannte Richtung vorliegt
        printf("Unknown direction enum value: %d\n", game->player_dir);
        game->dir_x = 0;
        game->dir_y = 0;
        game->plane_x = 0;
        game->plane_y = 0;
    }

    // Debug-Ausgaben
    printf("Direction: (%f, %f)\n", game->dir_x, game->dir_y);
    printf("Plane: (%f, %f)\n", game->plane_x, game->plane_y);
}

 
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
    game->mlx_connection = mlx_init();
    printf("\n%d nummer 1\n", game->player_dir);
    read_map(game, map);
    printf("\n%d nummer 2\n", game->player_dir);
    game->mlx_win = mlx_new_window(game->mlx_connection,
            game->win_x, game->win_y, "CUB3D");
    init_player_direction(game);
    printf("\n%d nummer 3\n", game->player_dir);
    raycasting(game);
    // minimap(game);

    mlx_loop_hook(game->mlx_connection, &render, game);
    mlx_hook(game->mlx_win, 2, 1L << 0, handle_key, game);
    mlx_hook(game->mlx_win, 17, 0, close_window, game);
    mlx_loop(game->mlx_connection);
}