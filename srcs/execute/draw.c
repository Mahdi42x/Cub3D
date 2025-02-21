/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:36:00 by mawada            #+#    #+#             */
/*   Updated: 2025/02/21 17:05:41 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	put_pixel_to_image(char *img_data, int x, int y, t_data *data)
{
	char	*pixel;

	if (x < 0 || y < 0
		|| x >= data->line_length_p / (data->bits_per_pixel_p / 8))
		return ;
	pixel = img_data
		+ (y * data->line_length_p + x * (data->bits_per_pixel_p / 8));
	*(unsigned int *)pixel = data->color_p;
}

// void	put_pixel_to_image(char *img_data, int x,
// 	int y, int color, int line_length, int bits_per_pixel)
// {
// 	char	*pixel;

// 	if (x < 0 || y < 0 || x >= line_length / (bits_per_pixel / 8))
// 		return ;
// 	pixel = img_data + (y * line_length + x * (bits_per_pixel / 8));
// 	*(unsigned int *)pixel = color;
// }

void	put_pixel(t_img_params *i, int x, int y, int color)
{
	if (x < 0 || y < 0 || x >= i->len / (i->bpp / 8))
		return ;
	*(unsigned int *)(i->img + (y * i->len + x * (i->bpp / 8))) = color;
}

static int	get_color(t_data *d, int x, int y)
{
	if (d->map[y][x] == '1')
		return (0xFFFFFF);
	return (0x000000);
}

void	draw_tile(t_data *d, t_img_params *i, int x, int y)
{
	int	c;
	int	a;
	int	b;

	c = get_color(d, x, y);
	a = 0;
	while (a < 10)
	{
		b = 0;
		while (b < 10)
		{
			put_pixel(i, x * 10 + a, y * 10 + b, c);
			b++;
		}
		a++;
	}
}

void	draw_map(t_data *d, t_img_params *i, int x, int y)
{
	if (d->map[y] && d->map[y][x])
	{
		draw_tile(d, i, x, y);
		if (d->map[y][x + 1])
			draw_map(d, i, x + 1, y);
		else
			draw_map(d, i, 0, y + 1);
	}
}
