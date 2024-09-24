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

typedef struct s_map
{
	char	**map;
	char	*no;
	char	*so;
	char	*we;
	char	*ea;
	int		*c;
	int		*f;
	int		fd;
	int		flag;
	double	width;
	double	height;
	char	map_face;
	double	map_px;
	double	map_py;
}	t_map;

typedef struct s_game
{
	void		*mlx_connection;
	void		*mlx_win;
	
    char        **layout;
    int         player_x;
    int         player_y;

    int        map_width;
    int        map_height;

	double      dir_x;            // X-Komponente der Blickrichtung des Spielers
    double      dir_y;            // Y-Komponente der Blickrichtung des Spielers
    double      plane_x;          // X-Komponente der Kameraebene (für FOV)
    double      plane_y;          // Y-Komponente der Kameraebene (für FOV)

	t_map		map;
    t_direction	player_dir;
}	t_game;

void	init_struct(t_game *game, char *map);
void	init_game(t_game *game,  char *map);

void	read_map(t_game *game, char *map);
void	ft_map(t_game *game, char *line, int *i);

void	raycasting(t_game *game);
void	draw_map(t_game *game);
void	draw_minimap(t_game *game);

void	move_player(int keycode, t_game *game);

void	init_player_direction(t_game *game);


void	open_window(t_game *game);
int		handle_key(int keycode, t_game *game);
int		close_window(t_game *game);

void	err(char *str);
int		check_format(char *str, char *check);


#endif
