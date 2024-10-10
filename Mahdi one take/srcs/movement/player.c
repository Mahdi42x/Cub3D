#include "../includes/cub3d.h"

// bool is_valid_move(t_game *game, int x, int y)
// {
//     if (x < 0 || x >= game->map_width || y < 0 || y >= game->map_height)
//         return false;
    
//     if (game->layout[y][x] == '1')
//         return false;
    
//     return true;
// }

// void update_player_position(t_game *game, int new_x, int new_y)
// {
//     if (is_valid_move(game, new_x, new_y))
//     {
//         game->player_x = new_x;
//         game->player_y = new_y;
        
//         // Debug-Ausgabe zur Überprüfung der neuen Position
//         printf("Player moved to position: (%d, %d)\n", game->player_x, game->player_y);
//     }
//     else
//     {
//         // Debug-Ausgabe zur Überprüfung ungültiger Bewegung
//         printf("Invalid move to position: (%d, %d)\n", new_x, new_y);
//     }
// }

// void move_player(int keycode, t_game *game)
// {
//     int new_x = game->player_x;
//     int new_y = game->player_y;

//     if (keycode == 119 || keycode == 126 || keycode == 65362) // W oder Pfeil nach oben
//         new_y--;
//     else if (keycode == 97 || keycode == 123 || keycode == 65361) // A oder Pfeil nach links
//         new_x--;
//     else if (keycode == 115 || keycode == 125 || keycode == 65364) // S oder Pfeil nach unten
//         new_y++;
//     else if (keycode == 100 || keycode == 124 || keycode == 65363) // D oder Pfeil nach rechts
//         new_x++;

//     update_player_position(game, new_x, new_y);

// }
