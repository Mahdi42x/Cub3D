/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   casting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emkalkan <emkalkan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:31:13 by emkalkan          #+#    #+#             */
/*   Updated: 2025/02/18 17:47:39 by emkalkan         ###   ########.fr       */
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
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	t_img_data	img;
	t_data		*data;
	t_img_params i;
	t_cross_params c;

	data = (t_data *)param;
	data->img = mlx_new_image(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	img.img_data = mlx_get_data_addr(data->img,
			&bits_per_pixel, &line_length, &endian);
	img.line_length = line_length;
	img.bits_per_pixel = bits_per_pixel;
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

void	render_weapon_loop(t_texture *weapon, t_img_data *img,
	t_weapon_draw *wd, t_renderweapon *rw)
{
	while (rw->y < wd->height)
	{
		rw->x = 0;
		while (rw->x < wd->width)
		{
			rw->tex_x = rw->x * weapon->width / wd->width;
			rw->tex_y = rw->y * weapon->height / wd->height;
			rw->color = *(int *)(weapon->addr + (rw->tex_y * weapon->line_length
						+ rw->tex_x * (weapon->bpp / 8)));
			if ((rw->color & 0x00FFFFFF) != 0)
				put_pixel_to_image(img->img_data, wd->start_x + rw->x,
					wd->start_y + rw->y, rw->color, img->line_length,
					img->bits_per_pixel);
			rw->x++;
		}
		rw->y++;
	}
}

void	render_weapon(t_data *data, char *img_data, int line_length,
	int bits_per_pixel)
{
	t_texture		*weapon;
	t_renderweapon	rw;
	t_weapon_draw	wd;
	t_img_data		img;

	weapon = &data->weapon_texture;
	if (!weapon->addr)
		return ;
	wd.width = weapon->width * 1.5;
	wd.height = weapon->height * 1.5;
	wd.start_x = WINDOW_WIDTH - wd.width;
	wd.start_y = WINDOW_HEIGHT - wd.height;
	rw.y = 0;
	img.img_data = img_data;
	img.line_length = line_length;
	img.bits_per_pixel = bits_per_pixel;
	render_weapon_loop(weapon, &img, &wd, &rw);
}
