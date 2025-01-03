/*
** cub3D Project Implementation with MiniLibX
**
** Full implementation of the project, including map parsing, raycasting, player movement, and a minimap.
*/

#include "include/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TILE_SIZE 64
#define PLAYER_SPEED 0.2
#define PLAYER_ROT_SPEED 0.05
#define MAP_WIDTH 10
#define MAP_HEIGHT 10


char *map[MAP_HEIGHT] = {
    "1111111111",
    "1000000001",
    "1000000001",
    "1000000001",
    "1000000001",
    "1000100001", // Place the player in the middle
    "1000000001",
    "1000000001",
    "1000000001",
    "1111111111",
};

typedef struct s_player {
    double x;
    double y;
    double dir_x;
    double dir_y;
    double plane_x;
    double plane_y;
} t_player;

typedef struct s_data {
    void *mlx;
    void *win;
    t_player player;
    int **buffer;
} t_data;

/* Key bindings */
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_LEFT 65363
#define KEY_RIGHT 65361
#define KEY_UP 65362
#define KEY_DOWN 65364
#define KEY_ESC 65307


/* Function prototypes */
void init_player(t_player *player);
int handle_keypress(int key, t_data *data);
void raycasting(t_data *data, char *img_data, int line_length, int bits_per_pixel);
int render(void *param);
int world_map(int x, int y);
void move_player(t_data *data, int key);
void rotate_player(t_data *data, int key);
void put_pixel_to_image(char *img_data, int x, int y, int color, int line_length, int bits_per_pixel);
void draw_minimap(t_data *data, char *img_data, int line_length, int bits_per_pixel);
int world_map(int x, int y);

/* Initialize player data */
void init_player(t_player *player) {
    player->x = 5.5; // Match the open area in the map
    player->y = 5.5;
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
}


/* Simple map checker */
int world_map(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        return (1);
    return (map[y][x] - '0');
}

/* Put a pixel into the image buffer */
void put_pixel_to_image(char *img_data, int x, int y, int color, int line_length, int bits_per_pixel) {
    char *pixel = img_data + (y * line_length + x * (bits_per_pixel / 8));
    *(unsigned int *)pixel = color;
}

/* Draw a minimap */
void draw_minimap(t_data *data, char *img_data, int line_length, int bits_per_pixel) {
    int scale = 10; // Scale of the minimap
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int color = (map[y][x] == '1') ? 0xFFFFFF : 0x000000; // White for walls, black for empty spaces
            for (int i = 0; i < scale; i++) {
                for (int j = 0; j < scale; j++) {
                    put_pixel_to_image(img_data, x * scale + i, y * scale + j, color, line_length, bits_per_pixel);
                }
            }
        }
    }
    // Draw the player on the minimap
    int player_x = (int)(data->player.x * scale);
    int player_y = (int)(data->player.y * scale);
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            put_pixel_to_image(img_data, player_x + i, player_y + j, 0xFF0000, line_length, bits_per_pixel);
        }
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

        int color = (side == 0) ? 0xFF0000 : 0x00FF00;
        for (int y = draw_start; y < draw_end; y++)
            put_pixel_to_image(img_data, x, y, color, line_length, bits_per_pixel);
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

int main(void) {
    t_data data;

    data.mlx = mlx_init();
    if (!data.mlx) {
        fprintf(stderr, "Error: Failed to initialize MiniLibX.\n");
        return (1);
    }
    data.win = mlx_new_window(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D");
    if (!data.win) {
        fprintf(stderr, "Error: Failed to create a window.\n");
        return (1);
    }
    init_player(&data.player);

    // Lock and hide the mouse at the start
    mlx_mouse_hide(data.mlx, data.win);
    mlx_mouse_move(data.mlx, data.win, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    // Hook events for keyboard and mouse
    mlx_hook(data.win, 2, 1L << 0, handle_keypress, &data); // Key press
    mlx_hook(data.win, 6, 1L << 6, handle_mouse, &data);    // Mouse move

    // Render loop
    mlx_loop_hook(data.mlx, render, &data);
    mlx_loop(data.mlx);

    return (0);
}
