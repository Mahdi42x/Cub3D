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

# define MINIMAP_SCALE 5  // Skalierungsfaktor für die Minimap
# define SIZE_X 1024
# define SIZE_Y 512

typedef enum {
    NORTH,
    SOUTH,
    EAST,
    WEST
} t_direction;


typedef struct s_game
{
	void		*mlx_connection;
	void		*mlx_win;
    int         win_x;
    int         win_y;
	
    char        **layout;
    int         player_x;
    int         player_y;

    int        map_width;
    int        map_height;

    double      pos_x;            // X-Position des Spielers in der Welt
    double      pos_y;            // Y-Position des Spielers in der Welt
    double      dir_x;            // X-Komponente der Blickrichtung des Spielers
    double      dir_y;            // Y-Komponente der Blickrichtung des Spielers
    double      plane_x;          // X-Komponente der Kameraebene (für FOV)
    double      plane_y;          // Y-Komponente der Kameraebene (für FOV)
    
    double      camera_y;         // Y-Koordinate der Kamera in der Welt
    double      camera_x;         // X-Koordinate der Kamera in der Welt
    double      ray_dir_x;        // X-Komponente der Richtung des Strahls
    double      ray_dir_y;        // Y-Komponente der Richtung des Strahls
    double      delta_dist_x;     // Länge des Strahls von einer X-Seite zur nächsten
    double      delta_dist_y;     // Länge des Strahls von einer Y-Seite zur nächsten
    double      side_dist_x;      // Länge des Strahls von der aktuellen Position zur nächsten X-Seite
    double      side_dist_y;      // Länge des Strahls von der aktuellen Position zur nächsten Y-Seite
    double      step_x;           // Schrittweite in X-Richtung
    double      step_y;           // Schrittweite in Y-Richtung
    int      map_x;            // X-Koordinate der aktuellen Zelle in der Karte
    int      map_y;            // Y-Koordinate der aktuellen Zelle in der Karte
    double      wall_dist;        // Abstand zur Wand
    int         wall_side;        // Seite der Wand (0 = X, 1 = Y)
    double      perpwalldist;     // Entfernung zur Wand

    int		rgb_floor[3];
	int		rgb_sky[3];


    t_direction player_dir;
    int     fd;
}	t_game;



void    init_struct(t_game *game, char *map);
void    init_game(t_game *game,  char *map);

void    read_map(t_game *game, char *map);
void	ft_map(t_game *game, char *line, int *i);

void    raycasting(t_game *game);
void    draw_minimap(t_game *game);

void move_player(int keycode, t_game *game);

void    init_player_direction(t_game *game);


void	open_window(t_game *game);
int     handle_key(int keycode, t_game *game);
int     close_window(t_game *game);

void	err(char *str);
int     check_format(char *str, char *check);


#endif
