/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:41:36 by mawada            #+#    #+#             */
/*   Updated: 2025/02/26 18:37:58 by mawada           ###   ########.fr       */
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
	if (d->no_path)
		printf("Texture NO path: %s\n", d->no_path);
	if (d->so_path)
		printf("Texture SO path: %s\n", d->so_path);
	if (d->we_path)
		printf("Texture WE path: %s\n", d->we_path);
	if (d->ea_path)
		printf("Texture EA path: %s\n", d->ea_path);
	if (d->no_path == NULL || d->so_path == NULL || d->we_path == NULL || d->ea_path == NULL)
	{
		fprintf(stderr, "Error: Missing texture path.\n");
		free_textures(d, 0);
		free_maps(d);
		free(d->map);
		mlx_destroy_display(d->mlx);
		free(d->mlx);
		get_next_line(-1);
		exit(1);
	}
}
