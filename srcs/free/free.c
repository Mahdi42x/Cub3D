/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:04:22 by mawada            #+#    #+#             */
/*   Updated: 2025/02/13 16:07:27 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	free_and_exit(void *ptr, const char *error_msg, int exit_code)
{
	if (ptr)
		free(ptr);
	if (error_msg)
		fprintf(stderr, "Error: %s\n", error_msg);
	exit(exit_code);
}

void	freeimg(t_data *data)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (data->textures[i].img)
		{
			mlx_destroy_image(data->mlx, data->textures[i].img);
			data->textures[i].img = NULL;
		}
		i++;
	}
	if (data->weapon_texture.img)
	{
		mlx_destroy_image(data->mlx, data->weapon_texture.img);
		data->weapon_texture.img = NULL;
	}
}

void	exit_game(t_data *data)
{
	if (data->mlx)
	{
		if (data->win)
			mlx_destroy_window(data->mlx, data->win);
		mlx_destroy_display(data->mlx);
		free(data->mlx);
		data->mlx = NULL;
	}
	exit(0);
}

void	exit_error(const char *error_msg, int exit_code)
{
	fprintf(stderr, "Error: %s\n", error_msg);
	exit(exit_code);
}

void	cleanup_mlx(t_data *data)
{
	if (data->mlx)
	{
		if (data->win)
		{
			mlx_destroy_window(data->mlx, data->win);
			data->win = NULL;
		}
		mlx_destroy_display(data->mlx);
		free(data->mlx);
	}
}
