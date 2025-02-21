/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:41:36 by mawada            #+#    #+#             */
/*   Updated: 2025/02/21 15:51:14 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	draw_player(t_img_params *i, int x, int y, int r)
{
	int	a;
	int	b;

	a = -r;
	while (a <= r)
	{
		b = -r;
		while (b <= r)
		{
			if (a * a + b * b <= r * r)
				put_pixel(i, x + a, y + b, 0xFF0000);
			b++;
		}
		a++;
	}
}

void	draw_direction(t_img_params *i, int x, int y, t_player *p)
{
	int	tip_x;
	int	tip_y;
	int	a;

	tip_x = x + (int)(10.0 * p->dir_x);
	tip_y = y + (int)(10.0 * p->dir_y);
	a = 0;
	while (a < 10)
	{
		put_pixel(i, x + a * (tip_x - x) / 10,
			y + a * (tip_y - y) / 10, 0xFFFFFF);
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
