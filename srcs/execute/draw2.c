/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:35:35 by mawada            #+#    #+#             */
/*   Updated: 2025/02/21 15:51:09 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	draw_minimap(t_data *d, char *img, int len, int bpp)
{
	t_img_params	i;

	i.img = img;
	i.len = len;
	i.bpp = bpp;
	draw_map(d, &i, 0, 0);
	draw_player(&i, (int)(d->player.x * 10), (int)(d->player.y * 10), 5);
	draw_direction(&i, (int)(d->player.x * 10),
		(int)(d->player.y * 10), &d->player);
}

void	calculate_step_direction(t_line_params *p, int *sx, int *sy)
{
	if (p->x0 < p->x1)
		*sx = 1;
	else
		*sx = -1;
	if (p->y0 < p->y1)
		*sy = 1;
	else
		*sy = -1;
}

void	draw_line_helper(t_img_params *i, t_line_params *p, int dx, int dy)
{
	int	sx;
	int	sy;
	int	err;
	int	e2;

	calculate_step_direction(p, &sx, &sy);
	err = dx - dy;
	while (1)
	{
		put_pixel(i, p->x0, p->y0, p->color);
		if (p->x0 == p->x1 && p->y0 == p->y1)
			break ;
		e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			p->x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			p->y0 += sy;
		}
	}
}

// void	draw_line_helper(t_img_params *i, t_line_params *p, int dx, int dy)
// {
// 	int	sx;
// 	int	sy;
// 	int	err;
// 	int	e2;

// 	if (p->x0 < p->x1)
// 		sx = 1;
// 	else
// 		sx = -1;
// 	if (p->y0 < p->y1)
// 		sy = 1;
// 	else
// 		sy = -1;
// 	err = dx - dy;
// 	while (1)
// 	{
// 		put_pixel(i, p->x0, p->y0, p->color);
// 		if (p->x0 == p->x1 && p->y0 == p->y1)
// 			break ;
// 		e2 = 2 * err;
// 		if (e2 > -dy)
// 		{
// 			err -= dy;
// 			p->x0 += sx;
// 		}
// 		if (e2 < dx)
// 		{
// 			err += dx;
// 			p->y0 += sy;
// 		}
// 	}
// }

void	draw_line(t_img_params *i, t_line_params *p)
{
	int	dx;
	int	dy;

	if (p->x1 >= p->x0)
		dx = p->x1 - p->x0;
	else
		dx = p->x0 - p->x1;
	if (p->y1 >= p->y0)
		dy = p->y1 - p->y0;
	else
		dy = p->y0 - p->y1;
	draw_line_helper(i, p, dx, dy);
}

void	draw_crosshair(t_img_params *i, t_cross_params *c)
{
	int	a;

	a = -10;
	while (a <= 10)
	{
		if (c->h / 2 + a >= 0 && c->h / 2 + a < c->h)
			put_pixel(i, c->w / 2, c->h / 2 + a, 0xFF69B4);
		if (c->w / 2 + a >= 0 && c->w / 2 + a < c->w)
			put_pixel(i, c->w / 2 + a, c->h / 2, 0xFF69B4);
		a++;
	}
}
