/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   casting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emkalkan <emkalkan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 16:48:15 by mawada            #+#    #+#             */
/*   Updated: 2025/02/16 18:41:43 by emkalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

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

void	calculate_texture(t_data *data, t_ray *ray)
{
	t_texture	*tex;

	if (ray->side == 0)
		ray->tex_num = ray->ray_dir_x > 0 ? 0 : 1;
	else
		ray->tex_num = ray->ray_dir_y > 0 ? 2 : 3;
	tex = &data->textures[ray->tex_num];
	if (!tex->addr)
	{
		fprintf(stderr, "Error: Invalid texture%d\n", ray->tex_num);
		fprintf(stderr, " address for texture %d\n", ray->tex_num);
		return ;
	}
	ray->wall_x = (ray->side == 0) ? data->player.y + ray->perp_wall_dist * ray->ray_dir_y
								   : data->player.x + ray->perp_wall_dist * ray->ray_dir_x;
	ray->wall_x -= floor(ray->wall_x);
	ray->tex_x = (int)(ray->wall_x * tex->width);
	ray->tex_x = tex->width - ray->tex_x - 1;
	if ((ray->side == 0 && ray->ray_dir_x > 0)
		|| (ray->side == 1 && ray->ray_dir_y < 0))
		ray->tex_x = tex->width - ray->tex_x - 1;
}

void	draw_ceiling(t_img_data *img, int x, t_data *data, int draw_start)
{
	int	y;

	y = 0;
	while (y < draw_start)
	{
		put_pixel_to_image(img->img_data, x, y, data->ceiling_color,
			img->line_length, img->bits_per_pixel);
		y++;
	}
}

void	draw_wall(t_img_data *img, int x, t_data *data, t_ray *ray)
{
	int			y;
	int			d;
	int			tex_y;
	int			color;
	t_texture	*tex;

	tex = &data->textures[ray->tex_num];
	y = ray->draw_start;
	while (y < ray->draw_end)
	{
		d = (y * 256) - (WINDOW_HEIGHT * 128) + (ray->line_height * 128);
		tex_y = ((d * tex->height) / ray->line_height) / 256;
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= tex->height)
			tex_y = tex->height - 1;
		color = *(int *)(tex->addr + (tex_y * tex->line_length
					+ ray->tex_x * (tex->bpp / 8)));
		if (ray->side == 1)
			color = (color >> 1) & 0x7F7F7F;
		put_pixel_to_image(img->img_data, x, y, color,
			img->line_length, img->bits_per_pixel);
		y++;
	}
}

void	draw_floor(t_img_data *img, int x, t_data *data, int draw_end)
{
	int	y;

	y = draw_end;
	while (y < WINDOW_HEIGHT)
	{
		put_pixel_to_image(img->img_data, x, y, data->floor_color,
			img->line_length, img->bits_per_pixel);
		y++;
	}
}

void	draw_pixels(t_data *data, t_img_data *img, int x, t_ray *ray)
{
	draw_ceiling(img, x, data, ray->draw_start);
	draw_wall(img, x, data, ray);
	draw_floor(img, x, data, ray->draw_end);
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

	data = (t_data *)param;
	data->img = mlx_new_image(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	img.img_data = mlx_get_data_addr(data->img,
			&bits_per_pixel, &line_length, &endian);
	img.line_length = line_length;
	img.bits_per_pixel = bits_per_pixel;
	raycasting(data, &img);
	draw_minimap(data, img.img_data, img.line_length, img.bits_per_pixel);
	render_weapon(data, img.img_data, img.line_length, img.bits_per_pixel);
	draw_crosshair(img.img_data, img.line_length,
		img.bits_per_pixel, WINDOW_WIDTH, WINDOW_HEIGHT);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	mlx_destroy_image(data->mlx, data->img);
	return (0);
}

void	render_weapon_loop(t_texture *weapon, t_img_data *img,
	int start_x, int weapon_width, int weapon_height, t_renderweapon *rw)
{
	while (rw->y < weapon_height)
	{
		rw->x = 0;
		while (rw->x < weapon_width)
		{
			rw->tex_x = rw->x * weapon->width / weapon_width;
			rw->tex_y = rw->y * weapon->height / weapon_height;
			rw->color = *(int *)(weapon->addr + (rw->tex_y * weapon->line_length + rw->tex_x * (weapon->bpp / 8)));
			if ((rw->color & 0x00FFFFFF) != 0)
				put_pixel_to_image(img->img_data, start_x + rw->x, rw->start_y + rw->y, rw->color, img->line_length, img->bits_per_pixel);
			rw->x++;
		}
		rw->y++;
	}
}

void	render_weapon(t_data *data, char *img_data, int line_length, int bits_per_pixel)
{
	t_texture		*weapon = &data->weapon_texture;
	t_renderweapon	rw;
	int				weapon_width;
	int				weapon_height;
	int				start_x;
	t_img_data		img;

	if (!weapon->addr)
		return ;
	weapon_width = weapon->width * 1.5;
	weapon_height = weapon->height * 1.5;
	start_x = WINDOW_WIDTH - weapon_width;
	rw.start_y = WINDOW_HEIGHT - weapon_height;
	rw.y = 0;
	img.img_data = img_data;
	img.line_length = line_length;
	img.bits_per_pixel = bits_per_pixel;
	render_weapon_loop(weapon, &img, start_x, weapon_width, weapon_height, &rw);
}
