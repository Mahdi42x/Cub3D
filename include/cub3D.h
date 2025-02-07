/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:04:34 by mawada            #+#    #+#             */
/*   Updated: 2025/02/07 16:52:09 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libs/mlx/mlx.h"
# include "../libs/libft/includes/ft_printf.h"
# include "../libs/libft/includes/get_next_line.h"
# include "../libs/libft/includes/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>

# define M_PI 3.14159265358979323846
# define WINDOW_WIDTH 1200
# define WINDOW_HEIGHT 900
# define TILE_SIZE 64
# define PLAYER_SPEED 0.125
# define PLAYER_ROT_SPEED 0.05

/* Key bindings */
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_LEFT 65363
# define KEY_RIGHT 65361
# define KEY_UP 65362
# define KEY_DOWN 65364
# define KEY_ESC 65307

typedef struct s_player{
	double	x;
	double	y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	double	angle;
	int		foundplayer;
}	t_player;

typedef struct s_texture {
	void	*img;
	char	*addr;
	int		width;
	int		height;
	int		bpp;
	int		line_length;
	int		endian;
}	t_texture;

typedef struct s_ray {
	double	camera_x;
	double	dir_x;
	double	dir_y;
	double	side_x;
	double	side_y;
	double	delta_x;
	double	delta_y;
	double	perp_dist;
	int		map_y;
	int		map_x;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
}	




t_ray;

// 3: NO, 2: SO, 1: WE, 0: EA
typedef struct s_data {
	t_texture	weapon_texture;
	t_player	player;
	t_texture	textures[4];
	void		*mlx;
	void		*win;
	char		**map;
	char		*no_path;
	char		*so_path;
	char		*we_path;
	char		*ea_path;
	int			**buffer;
	int			floor_color;
	int			ceiling_color;
	int			map_height;
	int			map_width;
	void		*img;
}	t_data;

typedef struct s_parsemap {
	size_t		len;
	char		**new_map;
	char		c;
	int			x;
}	t_parsemap;

typedef struct s_drawline {
	int	err;
	int	e2;
}	t_drawline;

typedef struct s_renderweapon {
	int			start_y;
	int			tex_x;
	int			tex_y;
	int			color;
	int			y;
	int			x;
}	t_renderweapon;

typedef struct s_flood
{
	int		rows;
	int		cols;
	char	**visited;
}t_flood;

typedef struct s_drawminimap {
	int		color;
	int		i;
	int		j;
	int		player_x;
	int		player_y;
	int		dx;
	int		dy;
	int		tip_x;
	int		tip_y;
	double	tip_length;
}	t_drawminimap;

/*/++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\*/
/*		 						 PARSING				 					*/
/*		 						 Map Parsing				 				*/
char	**parse_map(int fd);
char	**parse_map_from_line(char *first_map_line, int fd, t_data *data);
void	find_player(char **map, t_player *player);

void	handle_texture(char *line, t_data *data);
void	handle_color(char *line, t_data *data);
void	parse_maps(t_data *data, char *line, int fd);
char	**parse_map_from_line(char *first_map_line, int fd, t_data *data);

/*		 						 File Parsing				 				*/
void	parse_cub_file(t_data *data, const char *file_path);
void	parse_cub_file_helper(t_data *data);
int		is_cub_file(char *file_path);
int		parse_color(char *str, int i);
void	test_texture_loading(void *mlx, char *path, const char *label);
void	test_all_textures(t_data *data);

/*		 						 Fload fill				 					*/
int		has_player(char **map);
int		is_map_enclosed(char **map);
int		flood_fill(char **map, int x, int y, t_flood *flood);
int		check_map_validity(char **map, int *rows, int *cols);
int		flood_fill(char **map, int x, int y, t_flood *flood);
void	free_visited(char **visited, int rows);
char	**allocate_visited(int rows, int cols);

/*		 						 Load				 						*/
void	load_weapon_texture(t_data *data, char *path);
void	load_texture(t_data *data, t_texture *texture, char *line);

/*		 						 Init				 						*/
void	set_player_orientation(char direction, t_player *player);
void	init_data(t_data *data);

/*		 						 Input				 						*/
int		handle_focus(int event, void *param);
int		handle_mouse(int x, __attribute__((unused)) int y, t_data *data);
int		handle_keypress(int key, t_data *data);
/*/++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\*/
/*		 						 EXEC				 					*/
/*		 						 Movement				 					*/
void	move_player(t_data *data, int key);
void	rotate_player(t_data *data, int key);
int		world_map(t_data *data, int x, int y);
/*		 						 Casting				 					*/
void	raycasting(t_data *data, char *img_data,
			int line_length, int bits_per_pixel);
int		render(void *param);
void	render_weapon(t_data *data, char *img_data,
			int line_length, int bits_per_pixel);
/*		 						 Draw				 						*/
void	put_pixel_to_image(char *img_data, int x, int y, int color,
			int line_length, int bits_per_pixel);
void	draw_minimap(t_data *data, char *img_data,
			int line_length, int bits_per_pixel);
void	draw_line(char *img_data, int x0, int y0, int x1, int y1, int color,
			int line_length, int bits_per_pixel);
void	draw_crosshair(char *img_data, int line_length,
			int bits_per_pixel, int window_width, int window_height);
void	print_texture_paths(t_data *data);
/*/++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\*/
/*free*/
// void	free_textures(t_data *data);
// void	cleanup(t_data *data);
// void	free_map(char **map, int height);
// void	free_all(t_data *data);
// void	free_all_exit(t_data *data);
void	free_and_exit(void *ptr, const char *error_msg, int exit_code);
int		exit_game(t_data *data);
void	exit_error(const char *error_msg, int exit_code);
void	validate_player_spawn(int player_found);
void	check_player_spawn(char *row, int rows, t_data *data, int *player_found);
char	**realloc_map(char **map, int rows);
void	process_map_line(char ***map, char *line, int rows, t_data *data, int *player_found);
void	freeimg(t_data	*data);
void	cleanup_and_exit(t_data *data);



#endif
