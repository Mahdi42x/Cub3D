/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   casting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:31:13 by emkalkan          #+#    #+#             */
/*   Updated: 2025/02/21 15:35:17 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	dda_loop(t_data *data, t_ray *ray)
{
	while (!ray->hit)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (world_map(data, ray->map_x, ray->map_y))
			ray->hit = 1;
	}
}

void	perform_dda(t_data *data, t_ray *ray)
{
	init_dda_steps(data, ray);
	dda_loop(data, ray);
}

void	raycasting(t_data *data, t_img_data *img)
{
	int		x;
	t_ray	ray;

	x = 0;
	while (x < WINDOW_WIDTH)
	{
		init_raycasting(data, x, &ray);
		perform_dda(data, &ray);
		calculate_wall_distance(data, &ray);
		calculate_texture(data, &ray);
		draw_pixels(data, img, x, &ray);
		x++;
	}
}

int	render(void *param)
{
	t_img_data		img;
	t_data			*data;
	t_img_params	i;
	t_cross_params	c;

	data = (t_data *)param;
	data->img = mlx_new_image(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	img.img_data = mlx_get_data_addr(data->img,
			&data->bits_per_pixel_r, &data->line_length_r, &data->endian_r);
	img.line_length = data->line_length_r;
	img.bits_per_pixel = data->bits_per_pixel_r;
	raycasting(data, &img);
	draw_minimap(data, img.img_data, img.line_length, img.bits_per_pixel);
	render_weapon(data, img.img_data, img.line_length, img.bits_per_pixel);
	i.img = img.img_data;
	i.len = img.line_length;
	i.bpp = img.bits_per_pixel;
	c.w = WINDOW_WIDTH;
	c.h = WINDOW_HEIGHT;
	draw_crosshair(&i, &c);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	mlx_destroy_image(data->mlx, data->img);
	return (0);
}
