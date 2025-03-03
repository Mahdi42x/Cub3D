/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emkalkan <emkalkan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:42:53 by mawada            #+#    #+#             */
/*   Updated: 2025/03/03 18:58:09 by emkalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	set_player_orientation(char direction, t_player *player)
{
	if (direction == 'N')
	{
		player->foundplayer++;
		player->dir_y = -1;
		player->plane_x = 0.66;
	}
	else if (direction == 'S')
	{
		player->foundplayer++;
		player->dir_y = 1;
		player->plane_x = -0.66;
	}
	else if (direction == 'E')
	{
		player->foundplayer++;
		player->dir_x = 1;
		player->plane_y = 0.66;
	}
	else if (direction == 'W')
	{
		player->foundplayer++;
		player->dir_x = -1;
		player->plane_y = -0.66;
	}
}

void	init_data(t_data *data)
{
	data->player.x = 0;
	data->player.y = 0;
	data->player.dir_x = 0;
	data->player.dir_y = 0;
	data->player.plane_x = 0;
	data->player.plane_y = 0;
	data->player.foundplayer = 0;
	data->player.angle = 0;
	data->map_height = 100;
	data->ceiling_color = 0;
	data->floor_color = 0;
	data->map = 0;
	data->no_path = 0;
	data->so_path = 0;
	data->we_path = 0;
	data->ea_path = 0;
	data->textures[0].img = NULL;
	data->textures[1].img = NULL;
	data->textures[2].img = NULL;
	data->textures[3].img = NULL;
	data->weapon_texture.img = NULL;
	data->many = 0;
	data->many2 = 0;
	data->x_ff = 0;
	data->y_ff = 0;
}

void	init_raycasting(t_data *data, int x, t_ray *ray)
{
	ray->camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
	ray->ray_dir_x = data->player.dir_x + data->player.plane_x * ray->camera_x;
	ray->ray_dir_y = data->player.dir_y + data->player.plane_y * ray->camera_x;
	ray->map_x = (int)data->player.x;
	ray->map_y = (int)data->player.y;
	ray->delta_dist_x = fabs(1 / ray->ray_dir_x);
	ray->delta_dist_y = fabs(1 / ray->ray_dir_y);
	ray->hit = 0;
}

void	init_dda_steps(t_data *data, t_ray *ray)
{
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (data->player.x - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - data->player.x)
			*ray->delta_dist_x;
	}
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (data->player.y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - data->player.y)
			* ray->delta_dist_y;
	}
}
