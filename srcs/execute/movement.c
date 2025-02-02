/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:45:09 by mawada            #+#    #+#             */
/*   Updated: 2025/02/02 17:45:45 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	move_player(t_data *data, int key)
{
	t_player	*player;
	double		move_x;
	double		move_y;

	player = &data->player;
	move_x = 0.0;
	move_y = 0.0;
	if (key == KEY_W || key == KEY_UP)
	{
		move_x += player->dir_x * PLAYER_SPEED;
		move_y += player->dir_y * PLAYER_SPEED;
	}
	if (key == KEY_S || key == KEY_DOWN)
	{
		move_x -= player->dir_x * PLAYER_SPEED;
		move_y -= player->dir_y * PLAYER_SPEED;
	}
	if (key == KEY_A)
	{
		move_x -= player->plane_x * PLAYER_SPEED;
		move_y -= player->plane_y * PLAYER_SPEED;
	}
	if (key == KEY_D)
	{
		move_x += player->plane_x * PLAYER_SPEED;
		move_y += player->plane_y * PLAYER_SPEED;
	}
	if (!world_map(data, (int)(player->x + move_x), (int)(player->y)))
	{
		player->x += move_x;
	}
	if (!world_map(data, (int)(player->x), (int)(player->y + move_y)))
	{
		player->y += move_y;
	}
}

void	rotate_player(t_data *data, int key)
{
	t_player	*player;
	double		old_dir_x;
	double		old_plane_x;
	double		rot;

	player = &data->player;
	old_dir_x = player->dir_x;
	old_plane_x = player->plane_x;
	if (key == KEY_LEFT)
		rot = -PLAYER_ROT_SPEED;
	else
		rot = PLAYER_ROT_SPEED;
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

int	world_map(t_data *data, int x, int y)
{
	double	buffer;

	buffer = 0.5;
	if (x < 0 || y < 0 || !data->map
		|| !data->map[y] || x >= (int)strlen(data->map[y]))
	{
		return (1);
	}
	if (y < buffer || y >= (int)data->map_height - buffer
		|| x < buffer || x >= (int)data->map_width - buffer)
	{
		return (1);
	}
	if (data->map[y][x] == '1')
	{
		return (1);
	}
	else
	{
		return (0);
	}
}