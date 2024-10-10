#include "../includes/cub3d.h"

// static void draw_vertical_line(int x, int start, int end, t_game *game)
// {
//     int y = start;
//     while (y <= end)
//     {
//         // Zeichne eine rote Linie zur Überprüfung
//         mlx_pixel_put(game->mlx_connection, game->mlx_win, x, y, 0xFF0000); // Rot
//         y++;
//     }
// }



// static void clear_window(t_game *game) {
//     for (int x = 0; x < game->win_width; x++) {
//         for (int y = 0; y < game->win_height; y++) {
//             mlx_pixel_put(game->mlx_connection, game->mlx_win, x, y, 0x000000); // Hintergrund schwarz
//         }
//     }
// }

// void raycasting(t_game *game)
// {
//    printf("Raycasting...\n");
//    clear_window(game);
//    printf("Raycasting...e\n");
//     int x = 0;


//     while (x < game->win_width)
//     {
//         // Kamerawinkel basierend auf der aktuellen Spalte (x)
//         double camera_x = 2 * x / (double)game->win_width - 1;
//         t_vector ray_dir = {
//             game->dir.x + game->plane.x * camera_x,
//             game->dir.y + game->plane.y * camera_x
//         };

//         // Ray-Startposition (Spielerposition)
//         t_vector ray_pos = game->player_pos;

//         // Welches Feld in der Map
//         int map_x = (int)ray_pos.x;
//         int map_y = (int)ray_pos.y;

//         // Länge zur nächsten x- oder y-Achse
//         t_vector delta_dist = {
//             fabs(1 / ray_dir.x),
//             fabs(1 / ray_dir.y)
//         };

//         // Bestimme die Schritte und die Seitenabstände
//         t_vector side_dist;
//         int step_x, step_y;
//         int hit = 0;
//         int side;

//         if (ray_dir.x < 0)
//         {
//             step_x = -1;
//             side_dist.x = (ray_pos.x - map_x) * delta_dist.x;
//         }
//         else
//         {
//             step_x = 1;
//             side_dist.x = (map_x + 1.0 - ray_pos.x) * delta_dist.x;
//         }
//         if (ray_dir.y < 0)
//         {
//             step_y = -1;
//             side_dist.y = (ray_pos.y - map_y) * delta_dist.y;
//         }
//         else
//         {
//             step_y = 1;
//             side_dist.y = (map_y + 1.0 - ray_pos.y) * delta_dist.y;
//         }

//         // DDA-Algorithmus, um zu überprüfen, wo die Wand getroffen wird
//         while (hit == 0)
//         {
//             if (side_dist.x < side_dist.y)
//             {
//                 side_dist.x += delta_dist.x;
//                 map_x += step_x;
//                 side = 0;
//             }
//             else
//             {
//                 side_dist.y += delta_dist.y;
//                 map_y += step_y;
//                 side = 1;
//             }

//             // Wenn wir auf eine Wand stoßen
//             if (game->layout[map_x][map_y] == '1')
//                 hit = 1;
//             else if (game->layout[map_x][map_y] == '0') // Debug Ausgabe
//                printf("No wall at (%d, %d)\n", map_x, map_y);
//         }
         
//          printf("Raycasting at x=%d, map_x=%d, map_y=%d, hit=%d\n", x, map_x, map_y, hit);

//         // Entfernung zur Wand berechnen
//         double perp_wall_dist;
//         if (side == 0)
//             perp_wall_dist = (map_x - ray_pos.x + (1 - step_x) / 2) / ray_dir.x;
//         else
//             perp_wall_dist = (map_y - ray_pos.y + (1 - step_y) / 2) / ray_dir.y;

//         // Höhe der Linie für die Wand
//         int line_height = (int)(game->win_height / perp_wall_dist);

//         // Start- und Endpunkt für die Wand zeichnen
//         int draw_start = -line_height / 2 + game->win_height / 2;
//         if (draw_start < 0)
//             draw_start = 0;
//         int draw_end = line_height / 2 + game->win_height / 2;
//         if (draw_end >= game->win_height)
//             draw_end = game->win_height - 1;
         

//          printf("Drawing line at x: %d from %d to %d\n", x, draw_start, draw_end);
//          draw_vertical_line(x, draw_start, draw_end, game);

//         x++;
//     }
// }


// void rotate_view(t_game *game, double angle)
// {
//     double old_dir_x = game->dir.x;
//     game->dir.x = game->dir.x * cos(angle) - game->dir.y * sin(angle);
//     game->dir.y = old_dir_x * sin(angle) + game->dir.y * cos(angle);

//     double old_plane_x = game->plane.x;
//     game->plane.x = game->plane.x * cos(angle) - game->plane.y * sin(angle);
//     game->plane.y = old_plane_x * sin(angle) + game->plane.y * cos(angle);
// }
