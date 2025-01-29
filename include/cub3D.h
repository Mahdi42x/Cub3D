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
#define PLAYER_SPEED 0.125
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
    t_texture textures[4];  // 3: NO, 2: SO, 1: WE, 0: EA
	t_texture weapon_texture;
    int floor_color;
    int ceiling_color;
    char **map;
	int map_height;
    int map_width;
    char *no_path; // Pfad für NO
    char *so_path; // Pfad für SO
    char *we_path; // Pfad für WE
    char *ea_path; // Pfad für EA
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

/*/++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\*/
/*		 						 						PARSING 										 					*/
/*		 						 						Map Parsing											 				*/
char	 **parse_map(int fd);
char	 **parse_map_from_line(char *first_map_line, int fd, t_data *data);
void	 find_player(char **map, t_player *player);

/*		 						 						Fload fill											 				*/
int 	has_player(char **map);
int 	is_map_enclosed(char **map);
int 	flood_fill(char **map, int x, int y, int rows, int cols, char **visited);

/*		 						 						File Parsing					 				 					*/
void	 parse_cub_file(t_data *data, const char *file_path);
int		is_cub_file(char *file_path);
int 	parse_color(char *str, int i);
void	 test_texture_loading(void *mlx, char *path, const char *label);
void	 test_all_textures(t_data *data);

/*		 						 						Load					 						 					*/
void	 load_weapon_texture(t_data *data, char *path);
void	 load_texture(t_data *data, t_texture *texture, char *line);

/*		 						 						Init 					 						 					*/
void	 set_player_orientation(char direction, t_player *player);

/*		 						 						Input											 					*/
int 	handle_focus(int event, void *param);
int 	handle_mouse(int x, __attribute__((unused)) int y, t_data *data);
int 	handle_keypress(int key, t_data *data);
/*|++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++|*/
/*		 						 						EXEC 							 						 			*/
/*		 						 						Movement						 				 					*/
void	 move_player(t_data *data, int key);
void	 rotate_player(t_data *data, int key);
int 	world_map(t_data *data, int x, int y);
/*		 						 						Casting							 				 					*/
void	 raycasting(t_data *data, char *img_data, int line_length, int bits_per_pixel);
int 	render(void *param);
void	 render_weapon(t_data *data, char *img_data, int line_length, int bits_per_pixel);
/*		 						 						Draw 							 				 					*/
void	 put_pixel_to_image(char *img_data, int x, int y, int color, int line_length, int bits_per_pixel);
void	 draw_minimap(t_data *data, char *img_data, int line_length, int bits_per_pixel);
void	 draw_line(char *img_data, int x0, int y0, int x1, int y1, int color, int line_length, int bits_per_pixel);
void	 draw_crosshair(char *img_data, int line_length, int bits_per_pixel, int window_width, int window_height);
void	 print_texture_paths(t_data *data);
/*\++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++/*/
#endif
