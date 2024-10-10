#ifndef CUB3D_H
# define CUB3D_H

# include "../libs/libft/includes/libft.h"
# include "../libs/minilibx-linux/mlx.h"
# include <errno.h>
# include <fcntl.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <limits.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <X11/keysym.h>
# include <X11/X.h>
# include <math.h>

# define SIZE_X 1024
# define SIZE_Y 512
# define move_speed 0.1

# define TEXTURE_WIDTH 64  // Setze die Breite deiner Texturen
# define TEXTURE_HEIGHT 64 // Setze die Höhe deiner Texturen

typedef struct {
    double x;
    double y;
} t_vector;

typedef struct s_player
{
    double posX;
    double posY;
	t_vector pos;   // Position des Spielers
    t_vector dir;   // Richtung, in die der Spieler schaut
    t_vector plane; // Projektionsfläche des Spielers (Blickfeld)
} t_player;

typedef struct s_minimap
{
    void *win;
    int tile_size;
    int player_size;
    int width;
    int height;
} t_minimap;

typedef struct s_ray {
    double  posX;     // Spieler X-Position
    double  posY;     // Spieler Y-Position
    double  dirX;     // Richtung X
    double  dirY;     // Richtung Y
    // double  planeX;   // Kamera Ebene X
    // double  planeY;   // Kamera Ebene Y
	double	delta_dist_x;
	double	delta_dist_y;
	double	side_dist_x;
	double	side_dist_y;
	int		step_x;
	int		step_y;

	double	pos_x;
	double	pos_y;
	int		map_x;
	int		map_y;
	int		wall_side;
	double	wall_x;
	int		tex_x;
	int		tex_y;
	double	step;
	double	tex_pos;

	double	perpwalldist;
} t_ray;

typedef struct s_data {
    void    *mlx_ptr;
    void    *win_ptr;
    int     width;
    int     height;
    char    **layout;     // Layout der Karte

	char *floor_color_str;
    char *ceiling_color_str;

    int     player_x;    // Spieler X-Position
    int     player_y;    // Spieler Y-Position
    int     map_width;    // Breite der Karte
    int     map_height;   // Höhe der Karte
    int     fd;           // Dateideskriptor

    void    *north_texture; // NO Textur
    void    *south_texture; // SO Textur
    void    *west_texture;  // WE Textur
    void    *east_texture;  // EA Textur

	t_ray ray;    // Raycasting-Daten
	t_player	player;
	t_minimap	minimap;
} t_data;





#endif
