/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:12:56 by mawada            #+#    #+#             */
/*   Updated: 2025/02/11 16:15:40 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

int	handle_focus(int event, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (event == 9)
	{
		printf("Window focused.\n");
		mlx_mouse_move(data->mlx, data->win,
			WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	}
	else if (event == 10)
	{
		printf("Window lost focus.\n");
	}
	return (0);
}

int	handle_mouse(int x, __attribute__((unused)) int y, t_data *data)
{
	int			delta_x;
	double		rot;
	t_player	*player;
	double		old_dir_x;
	double		old_plane_x;

	if (x == WINDOW_WIDTH / 2) 
		return (0);
	mlx_mouse_move(data->mlx, data->win, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	delta_x = x - WINDOW_WIDTH / 2;
	rot = delta_x * 0.001;
	player = &data->player;
	old_dir_x = player->dir_x;
	old_plane_x = player->plane_x;
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

void	free_textures(t_data *data)
{
	free(data->map);
	if (data->mlx)
	{
		if (data->textures[0].img)
			mlx_destroy_image(data->mlx, data->textures[0].img);
		if (data->textures[1].img)
			mlx_destroy_image(data->mlx, data->textures[1].img);
		if (data->textures[2].img)
			mlx_destroy_image(data->mlx, data->textures[2].img);
		if (data->textures[3].img)
			mlx_destroy_image(data->mlx, data->textures[3].img);
		if (data->weapon_texture.img)
			mlx_destroy_image(data->mlx, data->weapon_texture.img);
	}
	free(data->no_path);
	free(data->so_path);
	free(data->we_path);
	free(data->ea_path);
	data->map = NULL;
}

int	handle_keypress(int key, t_data *data)
{
	int	i;

	if (key == KEY_ESC)
	{
		if (data->map)
		{
			i = 0;
			while (data->map[i])
			{
				free(data->map[i]);
				data->map[i] = NULL;
				i++;
			}
			free_textures(data);
		}
		exit_game(data);
	}
	if (key == KEY_W || key == KEY_A || key == KEY_S || key == KEY_D
		|| key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT
		|| key == KEY_RIGHT)
	{
		move_player(data, key);
	}
	return (0);
}
