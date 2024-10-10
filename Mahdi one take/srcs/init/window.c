#include "../includes/cub3d.h"


// int	close_window(t_game *game)
// {   
// 	ft_printf("Bye!\n");
// 	if (game->mlx_win)
// 		mlx_destroy_window(game->mlx_connection, game->mlx_win);
    
// 	// free_exit(game);
// 	free(game);
// 	exit(EXIT_SUCCESS);
// 	return (0);
// }

// int	handle_key(int keycode, t_game *game)
// {
// 	if (!game || !game->layout) // Überprüfen auf Nullzeiger
//     {
//         printf("Fehler: Spielzustand oder Layout nicht initialisiert.\n");
//         return -1;
//     }

// 	if (keycode == 53 || keycode == 27 || keycode == 9 || keycode == 65307)
// 		close_window(game);
// 	else if (keycode == 119) // W: Vorwärtsbewegung
//     {
//         if (game->layout[(int)(game->player_pos.x + game->dir.x * move_speed)][(int)game->player_pos.y] == '0')
//             game->player_pos.x += game->dir.x * move_speed;
//         if (game->layout[(int)game->player_pos.x][(int)(game->player_pos.y + game->dir.y * move_speed)] == '0')
//             game->player_pos.y += game->dir.y * move_speed;
//     }
//     else if (keycode == 115) // S: Rückwärtsbewegung
//     {
//         if (game->layout[(int)(game->player_pos.x - game->dir.x * move_speed)][(int)game->player_pos.y] == '0')
//             game->player_pos.x -= game->dir.x * move_speed;
//         if (game->layout[(int)game->player_pos.x][(int)(game->player_pos.y - game->dir.y * move_speed)] == '0')
//             game->player_pos.y -= game->dir.y * move_speed;
//     }
// 	else if (keycode == 65361) // Links drehen
//     	rotate_view(game, -0.05);
// 	else if (keycode == 65363) // Rechts drehen
//     	rotate_view(game, 0.05);

//     // raycasting(game); // Szene neu zeichnen
// 	// move_player(keycode, game);
// 	return (0);
// }
