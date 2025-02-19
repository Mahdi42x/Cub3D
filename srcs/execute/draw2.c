
#include "../../include/cub3D.h"

void	draw_minimap(t_data *d, char *img, int len, int bpp)
{
	t_img_params	i =	{img, len, bpp};

	draw_map(d, &i, 0, 0);
	draw_player(&i, (int)(d->player.x * 10), (int)(d->player.y * 10), 5);
	draw_direction(&i, (int)(d->player.x * 10),
			(int)(d->player.y * 10), &d->player);
}

void	draw_line_helper(t_img_params *i, t_line_params *p, int dx, int dy)
{
	int		sx;
	int		sy;
	int		err;
	int		e2;

	sx = (p->x0 < p->x1) ? 1 : -1;
	sy = (p->y0 < p->y1) ? 1 : -1;
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

void	draw_line(t_img_params *i, t_line_params *p)
{
	int	dx;
	int	dy;

	dx = (p->x1 >= p->x0) ? (p->x1 - p->x0) : (p->x0 - p->x1);
	dy = (p->y1 >= p->y0) ? (p->y1 - p->y0) : (p->y0 - p->y1);
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

void	print_texture_paths(t_data *d)
{
	printf("Texture NO path: %s\\n", d->no_path);
	printf("Texture SO path: %s\\n", d->so_path);
	printf("Texture WE path: %s\\n", d->we_path);
	printf("Texture EA path: %s\\n", d->ea_path);
}
