/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emkalkan <emkalkan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 15:48:06 by emkalkan          #+#    #+#             */
/*   Updated: 2025/02/17 16:18:29 by emkalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

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
