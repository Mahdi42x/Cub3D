/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emkalkan <emkalkan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:48:41 by mawada            #+#    #+#             */
/*   Updated: 2025/02/17 16:44:18 by emkalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	put_pixel_to_image(char *img_data, int x, int y, int color,
			int line_length, int bits_per_pixel)
{
	char	*dst;

	if (x < 0 || y < 0 || x >= WINDOW_WIDTH || y >= WINDOW_HEIGHT)
		return ;
	dst = img_data + (y * line_length + x * (bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

void	put_pixel(t_img_data *img, int x, int y, int color)
{
	char	*pixel;

	if (x < 0 || y < 0 || x >= WINDOW_WIDTH || y >= WINDOW_HEIGHT)
		return ;
	pixel = img->img_data + (y * img->line_length + x
			* (img->bits_per_pixel / 8));
	*(unsigned int *)pixel = color;
}

void	draw_minimap_tile(t_data *data, char *img_data, int x, int y, int scale, int line_length, int bits_per_pixel)
{
	int		i;
	int		j;
	int		color;

	color = (data->map[y][x] == '1') ? 0xFFFFFF : 0x000000;
	i = 0;
	while (i < scale)
	{
		j = 0;
		while (j < scale)
		{
			put_pixel_to_image(img_data, x * scale + i, y
				* scale + j, color, line_length, bits_per_pixel);
			j++;
		}
		i++;
	}
}

void	draw_player_on_minimap(t_data *data, t_img_data *img, int scale)
{
	int		dx;
	int		dy;
	int		radius;
	int		player_x;
	int		player_y;

	player_x = (int)(data->player.x * scale);
	player_y = (int)(data->player.y * scale);
	radius = 5;
	dy = -radius;
	while (dy <= radius)
	{
		dx = -radius;
		while (dx <= radius)
		{
			if (dx * dx + dy * dy <= radius * radius)
				put_pixel(img, player_x + dx, player_y + dy, 0xFF0000);
			dx++;
		}
		dy++;
	}
}

void	draw_minimap(t_data *data, char *img_data,
	int line_length, int bits_per_pixel)
{
	int			y;
	int			x;
	int			scale;
	t_img_data	img;

	img = {img_data, line_length, bits_per_pixel, data->rw};
	scale = 10;
	y = 0;
	while (data->map[y])
	{
		x = 0;
		while (data->map[y][x])
		{
			draw_minimap_tile(data, img_data, x, y, scale,
				line_length, bits_per_pixel);
			x++;
		}
		y++;
	}
	draw_player_on_minimap(data, &img, scale);
}

void	init_line_vars(t_drawline *line, int *dx,
	int *dy, int *sx, int *sy)
{
	if (*dx < 0)
		*dx = -(*dx);
	if (*dy < 0)
		*dy = -(*dy);
	*dy = -(*dy);
	*sx = (*dx > 0) ? 1 : -1;
	*sy = (*dy > 0) ? 1 : -1;
	line->err = *dx + *dy;
}

void	draw_line_loop(t_img_data *img, int x0, int y0,
	int x1, int y1, int color, t_drawline *line)
{
	int	dx = x1 - x0;
	int	dy = y1 - y0;
	int	sx = (dx > 0) ? 1 : -1;
	int	sy = (dy > 0) ? 1 : -1;

	while (1)
	{
		put_pixel(img, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break ;
		line->e2 = 2 * line->err;
		if (line->e2 >= dy * -1)
		{
			line->err += dy;
			x0 += sx;
		}
		if (line->e2 <= dx)
		{
			line->err += dx;
			y0 += sy;
		}
	}
}

void	draw_line(char *img_data, int x0, int y0, int x1, int y1, int color, int line_length, int bits_per_pixel)
{
	int			dx;
	int			dy;
	int			sx;
	int			sy;
	t_drawline	line;
	t_img_data	img = {img_data, line_length, bits_per_pixel, NULL};

	dx = x1 - x0;
	dy = y1 - y0;
	init_line_vars(&line, &dx, &dy, &sx, &sy);
	draw_line_loop(&img, x0, y0, x1, y1, color, &line);
}

void	draw_crosshair(char *img_data, int line_length, int bits_per_pixel, int window_width, int window_height)
{
	int	center_x = window_width / 2;
	int	center_y = window_height / 2;
	int	crosshair_size = 10;
	int	color = 0xFF69B4;
	int	y;
	int x;

	t_img_data img = {img_data, line_length, bits_per_pixel, NULL};
	y = center_y - crosshair_size;
	while (y <= center_y + crosshair_size)
	{
		if (y >= 0 && y < window_height)
			put_pixel(&img, center_x, y, color);
		y++;
	}
	x = center_x - crosshair_size;
	while (x <= center_x + crosshair_size)
	{
		if (x >= 0 && x < window_width)
			put_pixel(&img, x, center_y, color);
		x++;
	}
}

void	print_texture_paths(t_data *data)
{
	printf("Texture NO path: %s\\n", data->no_path);
	printf("Texture SO path: %s\\n", data->so_path);
	printf("Texture WE path: %s\\n", data->we_path);
	printf("Texture EA path: %s\\n", data->ea_path);
}
