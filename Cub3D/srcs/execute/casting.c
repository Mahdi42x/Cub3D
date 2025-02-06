/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   casting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 16:48:15 by mawada            #+#    #+#             */
/*   Updated: 2025/02/06 13:41:23 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	raycasting(t_data *data, char *img_data,
	int line_length, int bits_per_pixel)
{
	int	x;

	x = 0;
	while (x < WINDOW_WIDTH)
	{
		double		camera_x;
		double		ray_dir_x;
		double		ray_dir_y;
		int			map_x;
		int			map_y;
		double		side_dist_x;
		double		side_dist_y;
		double		delta_dist_x;
		double		delta_dist_y;
		double		perp_wall_dist;
		int			step_x;
		int			step_y;
		int			hit;
		int			side;

		camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
		ray_dir_x = data->player.dir_x + data->player.plane_x * camera_x;
		ray_dir_y = data->player.dir_y + data->player.plane_y * camera_x;
		map_x = (int)data->player.x;
		map_y = (int)data->player.y;
		delta_dist_x = fabs(1 / ray_dir_x);
		delta_dist_y = fabs(1 / ray_dir_y);
		hit = 0;
		if (ray_dir_x < 0)
		{
			step_x = -1;
			side_dist_x = (data->player.x - map_x) * delta_dist_x;
		}
		else
		{
			step_x = 1;
			side_dist_x = (map_x + 1.0 - data->player.x) * delta_dist_x;
		}
		if (ray_dir_y < 0)
		{
			step_y = -1;
			side_dist_y = (data->player.y - map_y) * delta_dist_y;
		}
		else
		{
			step_y = 1;
			side_dist_y = (map_y + 1.0 - data->player.y) * delta_dist_y;
		}
		while (!hit)
		{
			if (side_dist_x < side_dist_y)
			{
				side_dist_x += delta_dist_x;
				map_x += step_x;
				side = 0;
			}
			else
			{
				side_dist_y += delta_dist_y;
				map_y += step_y;
				side = 1;
			}
			if (world_map(data, map_x, map_y)) 
			{
				hit = 1;
			}
		}
		if (side == 0)
		{
			perp_wall_dist = (map_x - data->player.x + (1 - step_x)
					/ 2) / ray_dir_x;
		}
		else
		{
			perp_wall_dist = (map_y - data->player.y + (1 - step_y)
					/ 2) / ray_dir_y;
		}
		int			line_height;
		int			draw_start;
		int			draw_end;
		int			tex_num;
		t_texture	*tex;
		double		wall_x;
		int			tex_x;
		line_height = (int)(WINDOW_HEIGHT / perp_wall_dist);
		draw_start = -line_height / 2 + WINDOW_HEIGHT / 2;
		if (draw_start < 0)
			draw_start = 0;
		draw_end = line_height / 2 + WINDOW_HEIGHT / 2;
		if (draw_end >= WINDOW_HEIGHT)
			draw_end = WINDOW_HEIGHT - 1;
		if (side == 0)
		{
			if (ray_dir_x > 0)
			{
				tex_num = 0;
			}
			else
			{
				tex_num = 1;
			}
		}
		else
		{
			if (ray_dir_y > 0)
			{
				tex_num = 2;
			}
			else
			{
				tex_num = 3;
			}
		}
		tex = &data->textures[tex_num];
		if (!tex->addr)
		{
			fprintf(stderr,
				"Error: Invalid texture address for texture %d\n", tex_num);
			x++;
			continue ;
		}
		if (side == 0)
		{
			wall_x = data->player.y + perp_wall_dist * ray_dir_y;
		}
		else
		{
			wall_x = data->player.x + perp_wall_dist * ray_dir_x;
		}
		wall_x -= floor(wall_x);
		tex_x = (int)(wall_x * tex->width);
		tex_x = tex->width - tex_x - 1;
		if ((side == 0 && ray_dir_x > 0) || (side == 1 && ray_dir_y < 0))
		{
			tex_x = tex->width - tex_x - 1;
		}
		int y;

		y = 0;
		while (y < draw_start)
		{
			put_pixel_to_image(img_data, x, y,
				data->ceiling_color, line_length, bits_per_pixel);
			y++;
		}
		y = draw_start;
		while (y < draw_end)
		{
			int	d;
			int	tex_y;
			int	color;

			d = (y * 256) - (WINDOW_HEIGHT * 128) + (line_height * 128);
			tex_y = ((d * tex->height) / line_height) / 256;
			if (tex_y < 0)
				tex_y = 0;
			if (tex_y >= tex->height)
				tex_y = tex->height - 1;
			if (tex_x < 0)
				tex_x = 0;
			if (tex_x >= tex->width)
				tex_x = tex->width - 1;
			color = *(int *)(tex->addr + (tex_y * tex->line_length
						+ tex_x * (tex->bpp / 8)));
			if (side == 1)
			{
				color = (color >> 1) & 0x7F7F7F;
			}
			put_pixel_to_image(img_data, x, y, color,
				line_length, bits_per_pixel);
			y++;
		}
		y = draw_end;
		while (y < WINDOW_HEIGHT)
		{
			put_pixel_to_image(img_data, x, y, data->floor_color,
				line_length, bits_per_pixel);
			y++;
		}
		x++;
	}
}

int	render(void *param)
{
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	char	*img_data;
	t_data	*data;

	data = (t_data *)param;
	data->img = mlx_new_image(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	img_data = mlx_get_data_addr(data->img,
			&bits_per_pixel, &line_length, &endian);
	raycasting(data, img_data, line_length, bits_per_pixel);
	draw_minimap(data, img_data, line_length, bits_per_pixel);
	render_weapon(data, img_data, line_length, bits_per_pixel);
	draw_crosshair(img_data, line_length,
		bits_per_pixel, WINDOW_WIDTH, WINDOW_HEIGHT);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	mlx_destroy_image(data->mlx, data->img);
	return (0);
}

void	render_weapon(t_data *data, char *img_data, int line_length, int bits_per_pixel)
{
	t_texture		*weapon;
	int				weapon_width;
	int				weapon_height;
	int				start_x;
	t_renderweapon	renderweapon;

	weapon = &data->weapon_texture;
	if (!weapon->addr)
		return ;
	weapon_width = weapon->width * 1.5;
	weapon_height = weapon->height * 1.5;
	start_x = WINDOW_WIDTH - weapon_width;
	renderweapon.start_y = WINDOW_HEIGHT - weapon_height;
	renderweapon.y = 0;
	while (renderweapon.y < weapon_height)
	{
		renderweapon.x = 0;
		while (renderweapon.x < weapon_width)
		{
			renderweapon.tex_x = renderweapon.x * weapon->width / weapon_width;
			renderweapon.tex_y = renderweapon.y * weapon->height / weapon_height;
			renderweapon.color = *(int *)(weapon->addr + (renderweapon.tex_y * weapon->line_length + renderweapon.tex_x * (weapon->bpp / 8)));
			if ((renderweapon.color & 0x00FFFFFF) != 0)
				put_pixel_to_image(img_data, start_x + renderweapon.x, renderweapon.start_y + renderweapon.y, renderweapon.color, line_length, bits_per_pixel);
			renderweapon.x++;
		}
		renderweapon.y++;
	}
}
