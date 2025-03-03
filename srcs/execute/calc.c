/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 15:49:31 by emkalkan          #+#    #+#             */
/*   Updated: 2025/03/03 12:16:12 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	calculate_wall_distance(t_data *data, t_ray *ray)
{
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - data->player.x
				+ (1 - ray->step_x) / 2) / ray->ray_dir_x;
	else
		ray->perp_wall_dist = (ray->map_y - data->player.y
				+ (1 - ray->step_y) / 2) / ray->ray_dir_y;
	ray->line_height = (int)(WINDOW_HEIGHT / ray->perp_wall_dist);
	ray->draw_start = -ray->line_height / 2 + WINDOW_HEIGHT / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + WINDOW_HEIGHT / 2;
	if (ray->draw_end >= WINDOW_HEIGHT)
		ray->draw_end = WINDOW_HEIGHT - 1;
}

void	select_texture(t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->ray_dir_x > 0)
			ray->tex_num = 0;
		else
			ray->tex_num = 1;
	}
	else
	{
		if (ray->ray_dir_y > 0)
			ray->tex_num = 2;
		else
			ray->tex_num = 3;
	}
}

void	compute_texture_coordinates(t_data *data, t_ray *ray)
{
	t_texture	*tex;

	tex = &data->textures[ray->tex_num];
	if (!tex->addr)
	{
		printf("Error: Invalid texture %d\\n", ray->tex_num);
		printf("Address for texture %d\\n", ray->tex_num);
		free(ray);
	}
	if (ray->side == 0)
		ray->wall_x = data->player.y + ray->perp_wall_dist * ray->ray_dir_y;
	else
		ray->wall_x = data->player.x + ray->perp_wall_dist * ray->ray_dir_x;
	ray->wall_x -= floor(ray->wall_x);
	ray->tex_x = (int)(ray->wall_x * tex->width);
	ray->tex_x = tex->width - ray->tex_x - 1;
	if ((ray->side == 0 && ray->ray_dir_x > 0)
		|| (ray->side == 1 && ray->ray_dir_y < 0))
		ray->tex_x = tex->width - ray->tex_x - 1;
}

void	calculate_texture(t_data *data, t_ray *ray)
{
	select_texture(ray);
	compute_texture_coordinates(data, ray);
}
