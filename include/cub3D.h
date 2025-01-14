#ifndef CUB3D_H
# define CUB3D_H

#include "../libs/mlx/mlx.h"
#include "../libs/libft/includes/ft_printf.h"
#include "../libs/libft/includes/get_next_line.h"
#include "../libs/libft/includes/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define M_PI 3.14159265358979323846
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900
#define TILE_SIZE 64
#define PLAYER_SPEED 0.2
#define PLAYER_ROT_SPEED 0.05

typedef struct s_player {
    double x;
    double y;
    double dir_x;
    double dir_y;
    double plane_x;
    double plane_y;
	double angle;
} t_player;

typedef struct s_texture {
    void *img;
    char *addr;
    int width;
    int height;
    int bpp;
    int line_length;
    int endian;
} t_texture;

typedef struct s_data {
    void *mlx;
    void *win;
    t_player player;
    int **buffer;
    t_texture textures[4];  // 0: NO, 1: SO, 2: WE, 3: EA
	t_texture weapon_texture;
    int floor_color;
    int ceiling_color;
    char **map;
    int map_width;
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
void set_player_orientation(char direction, t_player *player);
void draw_line(char *img_data, int x0, int y0, int x1, int y1, int color, int line_length, int bits_per_pixel);
void init_player(t_player *player);
int handle_keypress(int key, t_data *data);
void raycasting(t_data *data, char *img_data, int line_length, int bits_per_pixel);
int render(void *param);
int world_map(t_data *data, int x, int y);
void move_player(t_data *data, int key);
void rotate_player(t_data *data, int key);
void put_pixel_to_image(char *img_data, int x, int y, int color, int line_length, int bits_per_pixel);
void draw_minimap(t_data *data, char *img_data, int line_length, int bits_per_pixel);

#endif