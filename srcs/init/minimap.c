#include "../includes/cub3d.h"

void draw_minimap(t_game *game)
{
    int x, y;
    int color;
    int border_thickness = 2; // Dicke der Umrandungslinie
    int pixel_size = 5;  // Größe jedes "Pixels" auf der Minimap

    // Position der Minimap im Fenster
    int minimap_offset_x = 10;
    int minimap_offset_y = 10;

    y = 0;
    while (y < game->map_height)
    {
        x = 0;
        while (x < game->map_width)
        {
            // Bestimme die Farbe basierend auf dem Inhalt der Karte
            if (game->layout[y][x] == '1')  // Wand
                color = 0xFFFFFF;  // Weiß
            else if (game->layout[y][x] == '0')  // Leerer Raum
                color = 0x000000;  // Schwarz
            else if (game->layout[y][x] == ' ')
                color = 0x000000;  // Standardfarbe für andere Zeichen

            // Zeichne ein Quadrat für jedes "Pixel" auf der Minimap
            int px, py;
            py = 0;
            while (py < pixel_size)
            {
                px = 0;
                while (px < pixel_size)
                {
                    mlx_pixel_put(game->mlx_connection, game->mlx_win, 
                                  x * pixel_size + px + minimap_offset_x + border_thickness, 
                                  y * pixel_size + py + minimap_offset_y + border_thickness, color);
                    px++;
                }
                py++;
            }

            x++;
        }
        y++;
    }

    // Zeichne den Spieler als größeres Quadrat
    int player_x_minimap = game->player_x * pixel_size + minimap_offset_x + border_thickness;
    int player_y_minimap = game->player_y * pixel_size + minimap_offset_y + border_thickness;
    int px, py;
    py = 0;
    while (py < pixel_size)
    {
        px = 0;
        while (px < pixel_size)
        {
            mlx_pixel_put(game->mlx_connection, game->mlx_win, 
                          player_x_minimap + px, 
                          player_y_minimap + py, 0x00FF00);  // Grüner Punkt für den Spieler
            px++;
        }
        py++;
    }

    // Zeichne die rote Umrandung
    int minimap_width = game->map_width * pixel_size;
    int minimap_height = game->map_height * pixel_size;
    int border_color = 0xFF0000;  // Rot

    // Obere und untere Grenze
    x = 0;
    while (x < minimap_width + 2 * border_thickness)
    {
        mlx_pixel_put(game->mlx_connection, game->mlx_win, 
                      x + minimap_offset_x, 
                      minimap_offset_y, border_color);  // Obere Grenze
        mlx_pixel_put(game->mlx_connection, game->mlx_win, 
                      x + minimap_offset_x, 
                      minimap_offset_y + minimap_height + border_thickness, border_color);  // Untere Grenze
        x++;
    }

    // Linke und rechte Grenze
    y = 0;
    while (y < minimap_height + 2 * border_thickness)
    {
        mlx_pixel_put(game->mlx_connection, game->mlx_win, 
                      minimap_offset_x, 
                      y + minimap_offset_y, border_color);  // Linke Grenze
        mlx_pixel_put(game->mlx_connection, game->mlx_win, 
                      minimap_offset_x + minimap_width + border_thickness, 
                      y + minimap_offset_y, border_color);  // Rechte Grenze
        y++;
    }
}


