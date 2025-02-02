/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:48:41 by mawada            #+#    #+#             */
/*   Updated: 2025/02/02 17:51:12 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"


void	put_pixel_to_image(char	*img_data, int x, int y, int color, int line_length, int bits_per_pixel)
{
	char	*pixel;

	pixel = img_data + (y * line_length + x * (bits_per_pixel / 8));
	*(unsigned int *)pixel = color;
}

void	draw_minimap(t_data *data, char	*img_data, int line_length, int bits_per_pixel)
{
	int				scale;
	int				radius;
	int				y;
	int				x;
	t_drawminimap	drawminimap;

	scale = 10;
	radius = 5;
	y = 0;
	while (data->map[y])
	{
		x = 0;
		while (data->map[y][x])
		{
			if (data->map[y][x] == '1')
				drawminimap.color = 0xFFFFFF;
			else
				drawminimap.color = 0x000000;
			drawminimap.i = 0;
			while (drawminimap.i < scale)
			{
				drawminimap.j = 0;
				while (drawminimap.j < scale)
				{
					put_pixel_to_image(img_data, x * scale + drawminimap.i,
						y * scale + drawminimap.j, drawminimap.color, line_length, bits_per_pixel);
					drawminimap.j++;
				}
				drawminimap.i++;
			}
			x++;
		}
		y++;
	}
	drawminimap.player_x = (int)(data->player.x * scale);
	drawminimap.player_y = (int)(data->player.y * scale);
	drawminimap.dy = -radius;
	while (drawminimap.dy <= radius)
	{
		drawminimap.dx = -radius;
		while (drawminimap.dx <= radius)
		{
			if (drawminimap.dx * drawminimap.dx + drawminimap.dy * drawminimap.dy <= radius * radius)
			{
				put_pixel_to_image(img_data,
					drawminimap.player_x + drawminimap.dx,
					drawminimap.player_y + drawminimap.dy,
					0xFF0000, line_length, bits_per_pixel);
			}
			drawminimap.dx++;
		}
		drawminimap.dy++;
	}
	drawminimap.tip_length = 10.0;
	drawminimap.tip_x = drawminimap.player_x + (int)(drawminimap.tip_length * data->player.dir_x);
	drawminimap.tip_y = drawminimap.player_y + (int)(drawminimap.tip_length * data->player.dir_y);
	draw_line(img_data, drawminimap.player_x, drawminimap.player_y, drawminimap.tip_x, drawminimap.tip_y,
		0xFFFFFF, line_length, bits_per_pixel);
}

void	draw_line(char *img_data, int x0, int y0, int x1, int y1, int color, int line_length, int bits_per_pixel)
{
	int			dx;
	int			dy;
	int			sx;
	int			sy;
	t_drawline	drawline;

	dx = x1 - x0;
	dy = y1 - y0;
	if (dx < 0)
		dx = -dx;
	if (dy < 0)
		dy = -dy;
	dy = -dy;
	if (x0 < x1)
		sx = 1;
	else
		sx = -1;
	if (y0 < y1)
		sy = 1;
	else
		sy = -1;
	drawline.err = dx + dy;
	while (1)
	{
		put_pixel_to_image(img_data, x0, y0, color, line_length, bits_per_pixel);
		if (x0 == x1 && y0 == y1)
		{
			break ;
		}
		drawline.e2 = 2 * drawline.err;
		if (drawline.e2 >= dy)
		{
			drawline.err += dy;
			x0 += sx;
		}
		if (drawline.e2 <= dx)
		{
			drawline.err += dx;
			y0 += sy;
		}
	}
}

void	draw_crosshair(char	*img_data, int line_length, int bits_per_pixel, int window_width, int window_height)
{
	int	center_x;
	int	center_y;	
	int	crosshair_size;
	int	color;

	center_x = window_width / 2;
	center_y = window_height / 2;
	crosshair_size = 10;
	color = 0xFF69B4;
	for (int y = center_y - crosshair_size; y <= center_y + crosshair_size; y++)
	{
		if (y >= 0 && y < window_height)
		{
			char	*pixel = img_data + (y * line_length + center_x * (bits_per_pixel / 8));
			*(unsigned int *)pixel = color;
		}
	}
	for (int x = center_x - crosshair_size; x <= center_x + crosshair_size; x++)
	{
		if (x >= 0 && x < window_width)
		{
			char	*pixel = img_data + (center_y * line_length + x * (bits_per_pixel / 8));
			*(unsigned int *)pixel = color;
		}
	}
}

void	print_texture_paths(t_data *data)
{
	printf("Texture NO path: %s\n", data->no_path);
	printf("Texture SO path: %s\n", data->so_path);
	printf("Texture WE path: %s\n", data->we_path);
	printf("Texture EA path: %s\n", data->ea_path);
}