#include "include/cub3D.h"

// void parse_map(t_data *data, int cub)
// {

// }

int parse_color(char *str) {
    int r, g, b;

    while (*str && isspace(*str)) // Skip leading spaces
        str++;
    r = atoi(str);
    str = strchr(str, ',') + 1;
    g = atoi(str);
    str = strchr(str, ',') + 1;
    b = atoi(str);

    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        fprintf(stderr, "Error: Invalid RGB value in .cub file.\n");
        exit(EXIT_FAILURE);
    }
    return (r << 16) | (g << 8) | b;
}

void parse_cub_file(t_data *data, const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("Error");
        fprintf(stderr, "Error: Could not open .cub file: %s\n", file_path);
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "F ", 2) == 0) {
            data->floor_color = parse_color(line + 2);
        } else if (strncmp(line, "C ", 2) == 0) {
            data->ceiling_color = parse_color(line + 2);
        }
        // Optionally parse other fields like textures (NO, SO, WE, EA)
    }
    fclose(file);
}

void set_player_orientation(char direction, t_player *player) {
    if (direction == 'N') {
        player->dir_x = 0;
        player->dir_y = -1;
    } else if (direction == 'S') {
        player->dir_x = 0;
        player->dir_y = 1;
    } else if (direction == 'E') {
        player->dir_x = 1;
        player->dir_y = 0;
    } else if (direction == 'W') {
        player->dir_x = -1;
        player->dir_y = 0;
    }
}

void find_player(char **map, t_player *player) {
    for (int y = 0; map[y]; y++) {
        for (int x = 0; map[y][x]; x++) {
            if (map[y][x] == 'N' || map[y][x] == 'S' || map[y][x] == 'E' || map[y][x] == 'W') {
                player->x = x + 0.5;  // Center of the block
                player->y = y + 0.5;
                set_player_orientation(map[y][x], player);
                map[y][x] = '0'; // Replace with empty space
                return;
            }
        }
    }
}

/* Initialize player data */
void init_player(t_player *player) {
    player->x = 1.1; // Match the open area in the map
    player->y = 1.1;
    player->dir_x = -1;
    player->dir_y = 0;
    player->plane_x = 0;
    player->plane_y = 0.66;
}

int handle_mouse(int x, __attribute__((unused)) int y, t_data *data) {
    static int center_x = WINDOW_WIDTH / 2;
    // Lock the mouse to the center of the window
    mlx_mouse_move(data->mlx, data->win, center_x, WINDOW_HEIGHT / 2);
    // Calculate horizontal movement
    int delta_x = x - center_x;
    double rot = -delta_x * 0.001; // Adjust sensitivity for smooth rotation
    t_player *player = &data->player;
    // Rotate the player's direction
    double old_dir_x = player->dir_x;
    double old_plane_x = player->plane_x;
    player->dir_x = player->dir_x * cos(rot) - player->dir_y * sin(rot);
    player->dir_y = old_dir_x * sin(rot) + player->dir_y * cos(rot);
    // Rotate the player's camera plane
    player->plane_x = player->plane_x * cos(rot) - player->plane_y * sin(rot);
    player->plane_y = old_plane_x * sin(rot) + player->plane_y * cos(rot);
    // Update player's angle
    player->angle -= rot;
    // Keep angle within 0 to 2*PI
    if (player->angle < 0)
        player->angle += 2 * M_PI;
    else if (player->angle >= 2 * M_PI)
        player->angle -= 2 * M_PI;
    return (0);
}
/* Handle keyboard input */
int handle_keypress(int key, t_data *data) {
    if (key == KEY_ESC) {
        mlx_destroy_window(data->mlx, data->win);
        exit(0);
    }
    if (key == KEY_W || key == KEY_A || key == KEY_S || key == KEY_D ||
        key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT) {
        move_player(data, key);
    }
    return (0);
}

void move_player(t_data *data, int key) {
    t_player *player = &data->player;
    double move_x = 0.0;
    double move_y = 0.0;

    // Handle forward/backward movement
    if (key == KEY_W || key == KEY_UP) {
        move_x += player->dir_x * PLAYER_SPEED;
        move_y += player->dir_y * PLAYER_SPEED;
    }
    if (key == KEY_S || key == KEY_DOWN) {
        move_x -= player->dir_x * PLAYER_SPEED;
        move_y -= player->dir_y * PLAYER_SPEED;
    }

    // Handle strafing left/right
    if (key == KEY_A) {
        move_x -= player->plane_x * PLAYER_SPEED;
        move_y -= player->plane_y * PLAYER_SPEED;
    }
    if (key == KEY_D) {
        move_x += player->plane_x * PLAYER_SPEED;
        move_y += player->plane_y * PLAYER_SPEED;
    }

    // Check for collisions and update position
    if (world_map((int)(player->x + move_x), (int)(player->y)) == 0)
        player->x += move_x;
    if (world_map((int)(player->x), (int)(player->y + move_y)) == 0)
        player->y += move_y;
}

/* Rotate the player */
void rotate_player(t_data *data, int key) {
    t_player *player = &data->player;
    double old_dir_x = player->dir_x;
    double old_plane_x = player->plane_x;
    double rot = (key == KEY_LEFT ? -PLAYER_ROT_SPEED : PLAYER_ROT_SPEED);
    player->dir_x = player->dir_x * cos(rot) - player->dir_y * sin(rot);
    player->dir_y = old_dir_x * sin(rot) + player->dir_y * cos(rot);
    player->plane_x = player->plane_x * cos(rot) - player->plane_y * sin(rot);
    player->plane_y = old_plane_x * sin(rot) + player->plane_y * cos(rot);
    player->angle += rot;
    // Keep angle within 0 to 2*PI
    if (player->angle < 0)
        player->angle += 2 * M_PI;
    else if (player->angle >= 2 * M_PI)
        player->angle -= 2 * M_PI;
}

/* Simple map checker */
int world_map(int x, int y) {
    // Check bounds first to prevent accessing invalid map positions
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        return 1;  // Treat out-of-bounds as walls

    // Return map value (0 for empty space, 1 for wall)
    return map[y][x] - '0';
}

/* Put a pixel into the image buffer */
void put_pixel_to_image(char *img_data, int x, int y, int color, int line_length, int bits_per_pixel) {
    char *pixel = img_data + (y * line_length + x * (bits_per_pixel / 8));
    *(unsigned int *)pixel = color;
}

/* Draw a minimap */
void draw_minimap(t_data *data, char *img_data, int line_length, int bits_per_pixel) {
    int scale = 10;  // Scale of the minimap

    // Flip the map vertically by reversing the Y-axis when drawing
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int color = (map[y][x] == '1') ? 0xFFFFFF : 0x000000;

            // Invert the Y-axis here
            int flipped_y = (MAP_HEIGHT - 1 - y);

            for (int i = 0; i < scale; i++) {
                for (int j = 0; j < scale; j++) {
                    put_pixel_to_image(img_data, x * scale + i, flipped_y * scale + j, color, line_length, bits_per_pixel);
                }
            }
        }
    }

    // Draw the player as a circle
    int player_x = (int)(data->player.x * scale);
    int player_y = (int)((MAP_HEIGHT - 1 - data->player.y) * scale);  // Invert player's Y-axis
    int radius = 5;  // Radius of the player circle

    // Draw the player circle
    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            if (dx * dx + dy * dy <= radius * radius) {  // Circle equation
                put_pixel_to_image(img_data, player_x + dx, player_y + dy, 0xFF0000, line_length, bits_per_pixel);  // Red circle
            }
        }
    }

    // Draw the directional tip (small line extending from the circle)
    double tip_length = 10.0;  // Length of the tip
    int tip_x = player_x + (int)(tip_length * data->player.dir_x);
    int tip_y = player_y - (int)(tip_length * data->player.dir_y);  // Invert Y-axis for the tip

    // Draw the tip as a line from the center of the circle
    draw_line(img_data, player_x, player_y, tip_x, tip_y, 0xFFFFFF, line_length, bits_per_pixel);  // White tip
}

void draw_line(char *img_data, int x0, int y0, int x1, int y1, int color, int line_length, int bits_per_pixel) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        put_pixel_to_image(img_data, x0, y0, color, line_length, bits_per_pixel);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

/* Raycasting implementation */
void raycasting(t_data *data, char *img_data, int line_length, int bits_per_pixel) {
    int x;
    for (x = 0; x < WINDOW_WIDTH; x++) {
        double camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
        double ray_dir_x = data->player.dir_x + data->player.plane_x * camera_x;
        double ray_dir_y = data->player.dir_y + data->player.plane_y * camera_x;

        int map_x = (int)data->player.x;
        int map_y = (int)data->player.y;

        double side_dist_x;
        double side_dist_y;

        double delta_dist_x = fabs(1 / ray_dir_x);
        double delta_dist_y = fabs(1 / ray_dir_y);
        double perp_wall_dist;

        int step_x;
        int step_y;

        int hit = 0;
        int side;

        if (ray_dir_x < 0) {
            step_x = -1;
            side_dist_x = (data->player.x - map_x) * delta_dist_x;
        } else {
            step_x = 1;
            side_dist_x = (map_x + 1.0 - data->player.x) * delta_dist_x;
        }
        if (ray_dir_y < 0) {
            step_y = -1;
            side_dist_y = (data->player.y - map_y) * delta_dist_y;
        } else {
            step_y = 1;
            side_dist_y = (map_y + 1.0 - data->player.y) * delta_dist_y;
        }

        while (hit == 0) {
            if (side_dist_x < side_dist_y) {
                side_dist_x += delta_dist_x;
                map_x += step_x;
                side = 0;
            } else {
                side_dist_y += delta_dist_y;
                map_y += step_y;
                side = 1;
            }
            if (world_map(map_x, map_y) > 0)
                hit = 1;
        }

        if (side == 0)
            perp_wall_dist = (map_x - data->player.x + (1 - step_x) / 2) / ray_dir_x;
        else
            perp_wall_dist = (map_y - data->player.y + (1 - step_y) / 2) / ray_dir_y;

        int line_height = (int)(WINDOW_HEIGHT / perp_wall_dist);
        int draw_start = -line_height / 2 + WINDOW_HEIGHT / 2;
        if (draw_start < 0)
            draw_start = 0;
        int draw_end = line_height / 2 + WINDOW_HEIGHT / 2;
        if (draw_end >= WINDOW_HEIGHT)
            draw_end = WINDOW_HEIGHT - 1;

        // Draw ceiling
        for (int y = 0; y < draw_start; y++) {
            put_pixel_to_image(img_data, x, y, data->ceiling_color, line_length, bits_per_pixel);
        }

        // Draw floor
        for (int y = draw_end; y < WINDOW_HEIGHT; y++) {
            put_pixel_to_image(img_data, x, y, data->floor_color, line_length, bits_per_pixel);
        }

        // Explicitly define wall color for now (you can add textures here later)
        int color = (side == 0) ? 0xFF0000 : 0x00FF00;
        for (int y = draw_start; y < draw_end; y++) {
            put_pixel_to_image(img_data, x, y, color, line_length, bits_per_pixel);
        }
    }
}

/* Render the scene */
int render(void *param) {
    t_data *data = (t_data *)param;

    // Declare variables for image properties
    int bits_per_pixel;
    int line_length;
    int endian;

    // Create a new image
    void *img = mlx_new_image(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    char *img_data = mlx_get_data_addr(img, &bits_per_pixel, &line_length, &endian);

    // Perform raycasting
    raycasting(data, img_data, line_length, bits_per_pixel);

    // Draw the minimap
    draw_minimap(data, img_data, line_length, bits_per_pixel);

    // Display the image
    mlx_put_image_to_window(data->mlx, data->win, img, 0, 0);

    // Destroy the image to free memory
    mlx_destroy_image(data->mlx, img);

    return (0);
}

int	is_cub_file(char *file_path)
{
	size_t	length;

	length = strlen(file_path);
	if (length >= 4)
	{
		if (strncmp(file_path + length - 4, ".cub", 4) == 0)
		{
			printf("map selected!\n");
			return (1);
		}
	}
	printf("The map does not have the correct ending '.cub'\n");
	return (0);
}
int main(int argc, char *argv[])
{
    t_data data;
    int fd;

    // Überprüfen, ob die Argumente korrekt sind
    if (argc != 2 || !is_cub_file(argv[1]))
    {
        printf("try: %s <map.cub>\n", argv[0]);
        return (1);
    }

    // Öffnen der .cub-Datei
    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        perror("Fehler beim Öffnen der Datei");
        return (1);
    }
	parse_cub_file(&data, argv[1]);
    // Map parsen und in data speichern
    //parse_map(&data, fd);
    //close(fd); // Datei nach dem Parsen schließen

    // MiniLibX initialisieren
    data.mlx = mlx_init();
    if (!data.mlx)
    {
        fprintf(stderr, "Error: Failed to initialize MiniLibX.\n");
        return (1);
    }

    // Erstellen eines Fensters
    data.win = mlx_new_window(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D");
    if (!data.win)
    {
        fprintf(stderr, "Error: Failed to create a window.\n");
        return (1);
    }

    // Spieler initialisieren
    init_player(&data.player);

    // Maus sperren und verbergen zu Beginn
    mlx_mouse_hide(data.mlx, data.win);
    mlx_mouse_move(data.mlx, data.win, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    // Events für Tastatur und Maus festlegen
    mlx_hook(data.win, 2, 1L << 0, handle_keypress, &data); // Tastendruck
    mlx_hook(data.win, 6, 1L << 6, handle_mouse, &data);    // Mausbewegung

    // Render-Schleife
    mlx_loop_hook(data.mlx, render, &data);
    mlx_loop(data.mlx);

    // Speicher für die Map nach der Verwendung freigeben
    for (int i = 0; data.map[i] != NULL; i++)
    {
        free(data.map[i]);
    }
    free(data.map);

    return (0);
}