#include "../includes/cub3d.h"

static void read_map(t_data *data, char *argv) {
    int i = 0;
    int line_number = 0; // Zähler für die Zeilennummer
    char *line;

    data->fd = open(argv, O_RDONLY);
    if (data->fd < 0) {
        perror("Error: Unable to open the map file");
        exit(EXIT_FAILURE);
    }

    data->layout = NULL; // Sicherstellen, dass layout initialisiert ist

    while ((line = get_next_line(data->fd)) != NULL) {
        printf("Read line: %s", line);
        line_number++; // Zeilennummer erhöhen

        // Extrahiere Texturpfade
        if (ft_strncmp(line, "NO", 2) == 0)
            data->north_texture = ft_strdup(ft_strtrim(line + 2, " \n"));
        else if (ft_strncmp(line, "SO", 2) == 0)
            data->south_texture = ft_strdup(ft_strtrim(line + 2, " \n"));
        else if (ft_strncmp(line, "WE", 2) == 0)
            data->west_texture = ft_strdup(ft_strtrim(line + 2, " \n"));
        else if (ft_strncmp(line, "EA", 2) == 0)
            data->east_texture = ft_strdup(ft_strtrim(line + 2, " \n"));
		else if (ft_strncmp(line, "F", 1) == 0)
            data->floor_color_str = ft_strdup(ft_strtrim(line + 1, " \n"));
        else if (ft_strncmp(line, "C", 1) == 0)
            data->ceiling_color_str = ft_strdup(ft_strtrim(line + 1, " \n"));
        else if (line_number >= 9 && (ft_strchr(line, '1') || ft_strchr(line, '0'))) { // Ab Zeile 9 speichern
            data->layout = realloc(data->layout, sizeof(char *) * (i + 1));
            if (!data->layout) {
                perror("Error: Memory allocation failed");
                exit(EXIT_FAILURE);
            }

            data->layout[i] = ft_strdup(line); // Kopiere die Zeile, um Speicherlecks zu vermeiden
            i++;

            // Überprüfe Spielerposition nur bei den gültigen Kartenzeilen
            char *player_position = NULL;
            if ((player_position = ft_strchr(line, 'N')) != NULL ||
                (player_position = ft_strchr(line, 'S')) != NULL ||
                (player_position = ft_strchr(line, 'E')) != NULL ||
                (player_position = ft_strchr(line, 'W')) != NULL) {
                data->player_x = player_position - line;
                data->player_y = i - 1; // Spielerposition wird in der layout-Zählung gesetzt
                printf("Player position: (%d, %d)\n", data->player_x, data->player_y); // Debug Ausgabe
            }
        }
        free(line); // Freigeben der Zeile, um Speicherlecks zu vermeiden
    }

    data->map_height = i;

    close(data->fd);

    if (data->map_height > 0) {
        data->map_width = strlen(data->layout[0]);
        printf("\nMap loaded: Width = %d, Height = %d\n", data->map_width, data->map_height); // Debug Ausgabe
    }
	printf("Floor color: %s\n", data->floor_color_str);
	printf("Ceiling color: %s\n", data->ceiling_color_str);
}





static void load_textures(t_data *data) {
    int width, height;

    data->north_texture = mlx_xpm_file_to_image(data->mlx_ptr, data->north_texture, &width, &height);
    data->south_texture = mlx_xpm_file_to_image(data->mlx_ptr, data->south_texture, &width, &height);
    data->west_texture = mlx_xpm_file_to_image(data->mlx_ptr, data->west_texture, &width, &height);
    data->east_texture = mlx_xpm_file_to_image(data->mlx_ptr, data->east_texture, &width, &height);

	if (data->north_texture || data->south_texture || data->west_texture || data->east_texture)
	{
		printf("\nTexturpfade gespeichert\n");
		printf("Texturpfad North: %p\n", data->north_texture);
		printf("Texturpfad East: %p\n", data->east_texture);
		printf("Texturpfad South: %p\n", data->south_texture);
		printf("Texturpfad West: %p\n", data->north_texture);
	}
}


// int hex_to_rgb(char *hex_str)
// {
//     int r, g, b;
//     sscanf(hex_str, "#%02x%02x%02x", &r, &g, &b);
//     return (r << 16) + (g << 8) + b;
// }

// void draw_floor_and_ceiling(t_data *data)
// {
//     int x, y;
    
//     // Zeichne die Decke
//     for (y = 0; y < data->height / 2; y++)
//     {
//         for (x = 0; x < data->width; x++)
//         {
//             mlx_pixel_put(data->mlx_ptr, data->win_ptr, x, y, hex_to_rgb(data->ceiling_color_str));
//         }
//     }
    
//     // Zeichne den Boden
//     for (y = data->height / 2; y < data->height; y++)
//     {
//         for (x = 0; x < data->width; x++)
//         {
//             mlx_pixel_put(data->mlx_ptr, data->win_ptr, x, y, hex_to_rgb(data->floor_color_str));
//         }
//     }
// }

// void render_walls(t_data *data)
// {
//     int x;
//     for (x = 0; x < data->width; x++)
//     {
//         // Raycasting Logik (vereinfacht)
//         int mapX = (int)data->player.posX;
//         int mapY = (int)data->player.posY;

//         if (data->layout[mapY][mapX] == '1')  // Wand in der Map
//         {
//             int drawStart = data->height / 4;  // Beispielwert für Start der Wand
//             int drawEnd = 3 * data->height / 4;  // Beispielwert für Ende der Wand

//             // Zeichne die Wand in rot
//             for (int y = drawStart; y < drawEnd; y++)
//             {
//                 mlx_pixel_put(data->mlx_ptr, data->win_ptr, x, y, 0xFF0000);  // Rot für Wände
//             }
//         }
//     }
// }

// static void raycasting(t_data *data)
// {
//     draw_floor_and_ceiling(data);  // Zeichne Boden und Decke
//     render_walls(data);            // Zeichne die Wände
// }

static void print_layout(t_data *data)
{
    int i, j;
    for (i = 0; i < data->map_height; i++)
    {
        for (j = 0; j < data->map_width; j++)
        {
            printf("%c", data->layout[i][j]);
        }
        // printf("\n");
    }
}

void draw_rect(t_data *data, int x, int y, int size, int color)
{
    int i, j;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            mlx_pixel_put(data->mlx_ptr, data->minimap.win, x + i, y + j, color);
        }
    }
}

void draw_player(t_data *data, int player_size)
{
    int player_x = (data->player_x * 10) + 5; // 5 als zusätzliche Anpassung für die genaue Zentrierung
    int player_y = (data->player_y * 10) + 5; // Ebenfalls +5 für die y-Achse

    draw_rect(data, player_x - player_size / 2, player_y - player_size / 2, player_size, 0xFF0000); // Spieler als roter Punkt
}



void draw_minimap(t_data *data)
{
    int map_tile_size = 10; // Größe einer Zelle in der Minimap
    int x, y;

    for (y = 0; y < data->map_height; y++)
    {
        for (x = 0; x < data->map_width; x++)
        {
            if (data->layout[y][x] == '1') // Wand
                draw_rect(data, x * map_tile_size, y * map_tile_size, map_tile_size, 0xFFFFFF); // Weiß für Wände
            else if (data->layout[y][x] == '0') // Leer
                draw_rect(data, x * map_tile_size, y * map_tile_size, map_tile_size, 0x000000); // Schwarz für Leer
        }
    }
	draw_player(data, 8);
}

void create_minimap_window(t_data *data)
{
    int minimap_width = data->map_width * 10; // Fensterbreite proportional zur Kartenbreite
    int minimap_height = data->map_height * 10; // Fensterhöhe proportional zur Kartenhöhe

    data->minimap.win = mlx_new_window(data->mlx_ptr, minimap_width, minimap_height, "Minimap");

    // Zeichne die Minimap in das neue Fenster
    draw_minimap(data);
}

void raycasting(t_data *data)
{
	(void) data;
	//versuch es zumachen
	printf("Raycasting\n");
}

int render(t_data *data) 
{
    // render_background(data);
    raycasting(data); // Diese Funktion musst du anpassen
    mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->north_texture, 0, 0); // Beispiel
    mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->south_texture, 0, 0);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->east_texture, 0, 0);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->west_texture, 0, 0);
	return (0);
}

// Alle anderen Funktionen musst du ebenfalls anpassen, um die Struktur von t_data zu verwenden.


int main(int argc, char **argv)
{
    t_data data;
    // t_ray ray;

    // Überprüfen der Argumente
    if (argc != 2)
    {
        printf("Usage: ./cub3d <map_file.cub>\n");
        return (1);
    }

	// Initialisierung des Spielers
    // ray.posX = 12; // Beispielkoordinaten
    // ray.posY = 12;

	// // Setze die Richtung der Kamera
    // ray.dirX = -1; // Beispielrichtung (nach links)
    // ray.dirY = 0;
    // ray.planeX = 0;  // Waagerechte Komponente der Kamera
    // ray.planeY = 0.66;  // Legt den FOV fest

    // Initialisierung der MinilibX und Fenster
    data.mlx_ptr = mlx_init();
    data.win_ptr = mlx_new_window(data.mlx_ptr, SIZE_X, SIZE_Y, "Cub3D");
    data.width = SIZE_X;
    data.height = SIZE_Y;

    // Einlesen der Karte
    read_map(&data, argv[1]);

	create_minimap_window(&data);

	printf("vor\n");
	print_layout(&data);
	printf("\nnach\n");
	load_textures(&data);
	if (!data.north_texture || !data.south_texture || !data.west_texture || !data.east_texture) 
	{
    	printf("Error: Unable to load one or more textures\n");
    	exit(EXIT_FAILURE);
	}

    // Initialisierung des Spielers
    // ray.posX = data.player_x + 0.5; // Spielerposition (Zentriert)
    // ray.posY = data.player_y + 0.5;
    // ray.dirX = -1;
    // ray.dirY = 0;
    // ray.planeX = 0;
    // ray.planeY = 0.66;

    // Raycasting
	mlx_loop_hook(&data.mlx_ptr, &render, &data);
    // raycasting(&data);
	// mlx_put_image_to_window(data.mlx_ptr, data.win_ptr, data.east_texture, 10, 10);

    mlx_loop(data.mlx_ptr);
    return (0);
}
