#ifndef CUB3D_H
# define CUB3D_H

# include "../libs/libft/includes/libft.h"
# include "../libs/minilibx-linux/mlx.h"
# include <errno.h>
# include <fcntl.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <X11/keysym.h>
# include <X11/X.h>

typedef struct s_game
{
	void		*mlx_connection;
	void		*mlx_win;
	
    int         player_x;
    int         player_y;
    
}	t_game;

void    init_struct(t_game *game, char *map);
void    init_game(t_game *game,  char *map);

void	open_window(t_game *game);
int     handle_key(int keycode, t_game *game);
int     close_window(t_game *game);

void	err(char *str);
int     check_format(char *str, char *check);


#endif
