#include "include/cub3D.h"

void load_weapon_texture(t_data *data, char *path) {
    data->weapon_texture.img = mlx_xpm_file_to_image(data->mlx, path, &data->weapon_texture.width, &data->weapon_texture.height);
    if (!data->weapon_texture.img) {
        perror("System Error");
        fprintf(stderr, "MiniLibX Error: Failed to load weapon texture at path: %s\n", path);
        exit(EXIT_FAILURE);
    }
    data->weapon_texture.addr = mlx_get_data_addr(
        data->weapon_texture.img,
        &data->weapon_texture.bpp,
        &data->weapon_texture.line_length,
        &data->weapon_texture.endian
    );
}

void load_texture(t_data *data, t_texture *texture, char *line) {
    // Skip leading spaces
    while (*line && isspace(*line))
        line++;

    // Remove trailing newline or spaces
    char *newline = strchr(line, '\n');
    if (newline)
        *newline = '\0';

    printf("🔍 Loading sanitized texture path: '%s'\n", line);

    // Load the texture
    texture->img = mlx_xpm_file_to_image(data->mlx, line, &texture->width, &texture->height);
    if (!texture->img) {
        perror("System Error");
        fprintf(stderr, "MiniLibX Error: Failed to load texture at path: %s\n", line);
        exit(EXIT_FAILURE);
    }

    texture->addr = mlx_get_data_addr(texture->img, &texture->bpp, &texture->line_length, &texture->endian);
}

char **parse_map(int fd) {
    char **map = NULL;
    char *line = NULL;
    int rows = 0;

    while ((line = get_next_line(fd)) != NULL) {
        // Skip leading empty lines only before the map starts
        if (rows == 0 && strspn(line, " \n") == strlen(line)) {
            free(line);
            continue;
        }

        // Preserve exact line formatting, including leading/trailing spaces
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // Allocate memory for each new map line
        char **new_map = realloc(map, sizeof(char *) * (rows + 2));
        if (new_map == NULL) {
            perror("Error reallocating map");
            exit(EXIT_FAILURE);
        }
        map = new_map;

        // Store the line exactly as it appears
        map[rows] = strdup(line);
        if (map[rows] == NULL) {
            perror("Error duplicating line");
            exit(EXIT_FAILURE);
        }
        rows++;
        map[rows] = NULL;  // Null-terminate the array

        free(line);
    }
    return map;
}

int parse_color(char *str, int i)
{
    int r, g, b;

    while (*str && isspace(*str)) // Skip leading spaces
        str++;
    r = atoi(str);
    str = strchr(str, ',') + 1;
    g = atoi(str);
    str = strchr(str, ',') + 1;
    b = atoi(str);
    // printf("Farbe: %i,%i,%i\n", r,g,b);

    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        fprintf(stderr, "Error: Invalid RGB value in .cub file.\n");
        exit(EXIT_FAILURE);
    }
    if (i == 0)
        printf("F: %i,%i,%i\n", r,g,b);
    else if (i == 1)
        printf("C: %i,%i,%i\n", r,g,b);
    return (r << 16) | (g << 8) | b;
}

char **parse_map_from_line(char *first_map_line, int fd, t_data *data) {
    char **map = NULL;
    char *line = first_map_line;
    int rows = 0;

    while (line != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // Allocate memory for the new map row
        char **new_map = realloc(map, sizeof(char *) * (rows + 2));
        if (new_map == NULL) {
            perror("Error reallocating map");
            exit(EXIT_FAILURE);
        }
        map = new_map;

        map[rows] = strdup(line);
        if (map[rows] == NULL) {
            perror("Error duplicating line");
            exit(EXIT_FAILURE);
        }

        // Find player position during parsing
        for (int x = 0; map[rows][x]; x++) {
		    char c = map[rows][x];
		    if (c == 'N' || c == 'S' || c == 'E' || c == 'W') {
		        data->player.x = x + 0.5;
		        data->player.y = rows + 0.5;
		        set_player_orientation(c, &data->player);
		        map[rows][x] = '0';  // Replace with empty space
		    }
		}

        rows++;
        map[rows] = NULL;  // Null-terminate the array

        free(line);
        line = get_next_line(fd);
    }

    return map;
}

void parse_cub_file(t_data *data, const char *file_path) {
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char *line;
    while ((line = get_next_line(fd)) != NULL) {
        if (strncmp(line, "NO ", 3) == 0) {
            load_texture(data, &data->textures[0], line + 3);  // North
        } else if (strncmp(line, "SO ", 3) == 0) {
            load_texture(data, &data->textures[1], line + 3);  // South
        } else if (strncmp(line, "WE ", 3) == 0) {
            load_texture(data, &data->textures[2], line + 3);  // West
        } else if (strncmp(line, "EA ", 3) == 0) {
            load_texture(data, &data->textures[3], line + 3);  // East
        } else if (strncmp(line, "F ", 2) == 0) {
            data->floor_color = parse_color(line + 2, 0);
        } else if (strncmp(line, "C ", 2) == 0) {
            data->ceiling_color = parse_color(line + 2, 1);
        } else if (*line == '1' || *line == '0' || *line == ' ') {
            data->map = parse_map_from_line(line, fd, data);
            break;
        }
        free(line);
    }

    close(fd);

    // Ensure the map was loaded before calculating map width
    if (data->map == NULL) {
        fprintf(stderr, "Error: Map data is missing in the .cub file.\n");
        exit(EXIT_FAILURE);
    }

    // Calculate the map width after loading the map
    int map_width = 0;
    int i = 0;
    while (data->map[i]) {
        int len = strlen(data->map[i]);
        if (len > map_width)
            map_width = len;
        i++;
    }
    data->map_width = map_width;
}

void set_player_orientation(char direction, t_player *player) {
    if (direction == 'N') {
        player->dir_x = 0;
        player->dir_y = -1;
        player->plane_x = 0.66;
        player->plane_y = 0;
    } else if (direction == 'S') {
        player->dir_x = 0;
        player->dir_y = 1;
        player->plane_x = -0.66;
        player->plane_y = 0;
    } else if (direction == 'E') {
        player->dir_x = 1;
        player->dir_y = 0;
        player->plane_x = 0;
        player->plane_y = 0.66;
    } else if (direction == 'W') {
        player->dir_x = -1;
        player->dir_y = 0;
        player->plane_x = 0;
        player->plane_y = -0.66;
    }
}

void find_player(char **map, t_player *player) {
    int y = 0;
    while (map[y]) {
        int x = 0;
        while (map[y][x]) {
            if (map[y][x] == 'N' || map[y][x] == 'S' || map[y][x] == 'E' || map[y][x] == 'W') {
                player->x = x + 0.5;  // Center the player in the tile
                player->y = y + 0.5;
                set_player_orientation(map[y][x], player);
                map[y][x] = '0';  // Replace with empty space
                return;
            }
            x++;
        }
        y++;
    }
}

int handle_focus(int event, void *param) {
    t_data *data = (t_data *)param;

    if (event == 9) {  // FocusIn
        printf("Window focused.\n");
        // Re-center the mouse when window regains focus
        mlx_mouse_move(data->mlx, data->win, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    } else if (event == 10) {  // FocusOut
        printf("Window lost focus.\n");
    }

    return (0);
}

int handle_mouse(int x, __attribute__((unused)) int y, t_data *data) {
    static int center_x = WINDOW_WIDTH / 2;

    if (x == center_x) 
        return (0);

    mlx_mouse_move(data->mlx, data->win, center_x, WINDOW_HEIGHT / 2);

    int delta_x = x - center_x;
    double rot = delta_x * 0.001;  // Inverted rotation direction

    t_player *player = &data->player;

    double old_dir_x = player->dir_x;
    double old_plane_x = player->plane_x;

    // Corrected rotation direction
    player->dir_x = player->dir_x * cos(rot) - player->dir_y * sin(rot);
    player->dir_y = old_dir_x * sin(rot) + player->dir_y * cos(rot);

    player->plane_x = player->plane_x * cos(rot) - player->plane_y * sin(rot);
    player->plane_y = old_plane_x * sin(rot) + player->plane_y * cos(rot);

    player->angle += rot;
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
    if (!world_map(data, (int)(player->x + move_x), (int)(player->y))) {
    	player->x += move_x;
	}
	if (!world_map(data, (int)(player->x), (int)(player->y + move_y))) {
    	player->y += move_y;
	}

}

/* Rotate the player */
void rotate_player(t_data *data, int key) {
    t_player *player = &data->player;
    double old_dir_x = player->dir_x;
    double old_plane_x = player->plane_x;
    double rot = (key == KEY_LEFT ? -PLAYER_ROT_SPEED : PLAYER_ROT_SPEED);  // Inverted rotation

    player->dir_x = player->dir_x * cos(rot) - player->dir_y * sin(rot);
    player->dir_y = old_dir_x * sin(rot) + player->dir_y * cos(rot);

    player->plane_x = player->plane_x * cos(rot) - player->plane_y * sin(rot);
    player->plane_y = old_plane_x * sin(rot) + player->plane_y * cos(rot);

    player->angle += rot;
    if (player->angle < 0)
        player->angle += 2 * M_PI;
    else if (player->angle >= 2 * M_PI)
        player->angle -= 2 * M_PI;
}

int world_map(t_data *data, int x, int y) {
    // Define a buffer space for collision near outer walls
    double buffer = 0.5;

    // Check for out-of-bounds access
    if (x < 0 || y < 0 || !data->map || !data->map[y] || x >= (int)strlen(data->map[y])) {
        return 1; // Out-of-bounds counts as a wall
    }

    // Adjust collision detection for outer walls
    if (y < buffer || y >= (int)data->map_height - buffer || x < buffer || x >= (int)data->map_width - buffer) {
        return 1; // Treat positions near any outer wall as a collision
    }

    // Standard wall check
    return (data->map[y][x] == '1') ? 1 : 0;
}

/* Put a pixel into the image buffer */
void put_pixel_to_image(char *img_data, int x, int y, int color, int line_length, int bits_per_pixel) {
    char *pixel = img_data + (y * line_length + x * (bits_per_pixel / 8));
    *(unsigned int *)pixel = color;
}

/* Draw a minimap */
void draw_minimap(t_data *data, char *img_data, int line_length, int bits_per_pixel) {
    int scale = 10;  // Scale of the minimap
    int radius = 5;  // Radius of the player circle

    // Draw the map
    int y = 0;
    while (data->map[y]) {
        int x = 0;
        while (data->map[y][x]) {
            int color = (data->map[y][x] == '1') ? 0xFFFFFF : 0x000000;

            int i = 0;
            while (i < scale) {
                int j = 0;
                while (j < scale) {
                    put_pixel_to_image(img_data, x * scale + i, y * scale + j, color, line_length, bits_per_pixel);
                    j++;
                }
                i++;
            }
            x++;
        }
        y++;
    }

    // Draw the player in the correct position
    int player_x = (int)(data->player.x * scale);
    int player_y = (int)(data->player.y * scale);

    // Draw the player circle
    int dy = -radius;
    while (dy <= radius) {
        int dx = -radius;
        while (dx <= radius) {
            if (dx * dx + dy * dy <= radius * radius) {
                put_pixel_to_image(img_data, player_x + dx, player_y + dy, 0xFF0000, line_length, bits_per_pixel);
            }
            dx++;
        }
        dy++;
    }

    // Draw the directional tip (invert Y-axis to fix minimap direction)
    double tip_length = 10.0;
    int tip_x = player_x + (int)(tip_length * data->player.dir_x);
    int tip_y = player_y + (int)(tip_length * data->player.dir_y);

    draw_line(img_data, player_x, player_y, tip_x, tip_y, 0xFFFFFF, line_length, bits_per_pixel);
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
    for (int x = 0; x < WINDOW_WIDTH; x++) {
        double camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
        double ray_dir_x = data->player.dir_x + data->player.plane_x * camera_x;
        double ray_dir_y = data->player.dir_y + data->player.plane_y * camera_x;

        int map_x = (int)data->player.x;
        int map_y = (int)data->player.y;

        double side_dist_x, side_dist_y;
        double delta_dist_x = fabs(1 / ray_dir_x);
        double delta_dist_y = fabs(1 / ray_dir_y);
        double perp_wall_dist;

        int step_x, step_y, hit = 0, side;

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

        while (!hit) {
            if (side_dist_x < side_dist_y) {
                side_dist_x += delta_dist_x;
                map_x += step_x;
                side = 0;
            } else {
                side_dist_y += delta_dist_y;
                map_y += step_y;
                side = 1;
            }
            if (world_map(data, map_x, map_y)) hit = 1;
        }

        perp_wall_dist = (side == 0)
            ? (map_x - data->player.x + (1 - step_x) / 2) / ray_dir_x
            : (map_y - data->player.y + (1 - step_y) / 2) / ray_dir_y;

        int line_height = (int)(WINDOW_HEIGHT / perp_wall_dist);
        int draw_start = -line_height / 2 + WINDOW_HEIGHT / 2;
        if (draw_start < 0) draw_start = 0;
        int draw_end = line_height / 2 + WINDOW_HEIGHT / 2;
        if (draw_end >= WINDOW_HEIGHT) draw_end = WINDOW_HEIGHT - 1;

        int tex_num = side == 0 ? (ray_dir_x > 0 ? 0 : 1) : (ray_dir_y > 0 ? 2 : 3);
        t_texture *tex = &data->textures[tex_num];

        // Check texture validity
        if (!tex->addr) {
            fprintf(stderr, "Error: Invalid texture address for texture %d\n", tex_num);
            continue;
        }

        double wall_x = side == 0
            ? data->player.y + perp_wall_dist * ray_dir_y
            : data->player.x + perp_wall_dist * ray_dir_x;
        wall_x -= floor(wall_x);

        int tex_x = (int)(wall_x * tex->width);
        if ((side == 0 && ray_dir_x > 0) || (side == 1 && ray_dir_y < 0))
            tex_x = tex->width - tex_x - 1;

        // Draw ceiling
        for (int y = 0; y < draw_start; y++) {
            put_pixel_to_image(img_data, x, y, data->ceiling_color, line_length, bits_per_pixel);
        }

        // Draw wall with texture
        for (int y = draw_start; y < draw_end; y++) {
    		int d = (y * 256) - (WINDOW_HEIGHT * 128) + (line_height * 128);

    		if (line_height > 0) {
    		    int tex_y = ((d * tex->height) / line_height) / 256;

    		    // Clamp tex_y to ensure it is within valid bounds
    		    if (tex_y < 0) tex_y = 0;
    		    if (tex_y >= tex->height) tex_y = tex->height - 1;

    		    // Ensure tex_x is also valid (should already be, but for safety):
    		    if (tex_x < 0) tex_x = 0;
    		    if (tex_x >= tex->width) tex_x = tex->width - 1;

    		    int color = *(int *)(tex->addr + (tex_y * tex->line_length + tex_x * (tex->bpp / 8)));
    		    if (side == 1) color = (color >> 1) & 0x7F7F7F;  // Darken side walls
    		    put_pixel_to_image(img_data, x, y, color, line_length, bits_per_pixel);
    		} else {
        	fprintf(stderr, "Error: Invalid line_height (%d)\n", line_height);
    }
}

        // Draw floor
        for (int y = draw_end; y < WINDOW_HEIGHT; y++) {
            put_pixel_to_image(img_data, x, y, data->floor_color, line_length, bits_per_pixel);
        }
    }
}

void render_weapon(t_data *data, char *img_data, int line_length, int bits_per_pixel) {
    t_texture *weapon = &data->weapon_texture;

    float weapon_scale = 1.5;  // Adjust this value to control size
    int weapon_width = weapon->width * weapon_scale;
    int weapon_height = weapon->height * weapon_scale;

    // 📌 Position the weapon at the bottom-right corner
    int start_x = WINDOW_WIDTH - weapon_width;  // 20px padding from the right edge
    int start_y = WINDOW_HEIGHT - weapon_height;  // 20px padding from the bottom edge

    for (int y = 0; y < weapon_height; y++) {
        for (int x = 0; x < weapon_width; x++) {
            int tex_x = x * weapon->width / weapon_width;
            int tex_y = y * weapon->height / weapon_height;

            int color = *(int *)(weapon->addr + (tex_y * weapon->line_length + tex_x * (weapon->bpp / 8)));

            // Skip transparent pixels (assuming transparency is black)
            if (color != 0x000000)
                put_pixel_to_image(img_data, start_x + x, start_y + y, color, line_length, bits_per_pixel);
        }
    }
}

void draw_crosshair(char *img_data, int line_length, int bits_per_pixel, int window_width, int window_height) {
    int center_x = window_width / 2;
    int center_y = window_height / 2;
    int crosshair_size = 10; // Length of the crosshair lines
    int color = 0xFF0000; // Red color for the crosshair

    // Draw vertical line
    for (int y = center_y - crosshair_size; y <= center_y + crosshair_size; y++) {
        if (y >= 0 && y < window_height) { // Ensure within bounds
            char *pixel = img_data + (y * line_length + center_x * (bits_per_pixel / 8));
            *(unsigned int *)pixel = color;
        }
    }

    // Draw horizontal line
    for (int x = center_x - crosshair_size; x <= center_x + crosshair_size; x++) {
        if (x >= 0 && x < window_width) { // Ensure within bounds
            char *pixel = img_data + (center_y * line_length + x * (bits_per_pixel / 8));
            *(unsigned int *)pixel = color;
        }
    }
}

/* Render the scene */
int render(void *param) {
    t_data *data = (t_data *)param;

    int bits_per_pixel;
    int line_length;
    int endian;

    void *img = mlx_new_image(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    char *img_data = mlx_get_data_addr(img, &bits_per_pixel, &line_length, &endian);

    // Render the game world
    raycasting(data, img_data, line_length, bits_per_pixel);

    // Draw the minimap (if desired)
    draw_minimap(data, img_data, line_length, bits_per_pixel);

    // Render the weapon
    render_weapon(data, img_data, line_length, bits_per_pixel);

	draw_crosshair(img_data, line_length, bits_per_pixel, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Show the image
    mlx_put_image_to_window(data->mlx, data->win, img, 0, 0);

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
            printf("\n");
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


    // MiniLibX initialisieren
    data.mlx = mlx_init();
    if (!data.mlx)
    {
        fprintf(stderr, "Error: Failed to initialize MiniLibX.\n");
        return (1);
    }
	int width, height;
    void *img = mlx_xpm_file_to_image(data.mlx, "/home/emkalkan/42/Cub3D-main/textures/Brick256.xpm", &width, &height);

    if (!img) {
        perror("System Error");
        fprintf(stderr, "MiniLibX Error: Cannot load the test texture.\n");
        exit(EXIT_FAILURE);
    } else {
        printf("✅ MiniLibX successfully loaded the test texture!\n");
        mlx_destroy_image(data.mlx, img);  // Clean up after the test
    }

	parse_cub_file(&data, argv[1]);
	load_weapon_texture(&data, "textures/m4default.xpm");


    // Erstellen eines Fensters
    data.win = mlx_new_window(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D");
    if (!data.win)
    {
        fprintf(stderr, "Error: Failed to create a window.\n");
        return (1);
    }

    // Maus sperren und verbergen zu Beginn
    mlx_mouse_hide(data.mlx, data.win);
    mlx_mouse_move(data.mlx, data.win, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    // Events für Tastatur und Maus festlegen
    mlx_hook(data.win, 2, 1L << 0, handle_keypress, &data); // Tastendruck
    mlx_hook(data.win, 6, 1L << 6, handle_mouse, &data);    // Mausbewegung
	mlx_hook(data.win, 9, 0, handle_focus, &data);  // FocusIn
	mlx_hook(data.win, 10, 0, handle_focus, &data); // FocusOut


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