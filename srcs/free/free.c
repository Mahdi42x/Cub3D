/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:04:22 by mawada            #+#    #+#             */
/*   Updated: 2025/02/07 17:21:05 by mawada           ###   ########.fr       */
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

void	freeimg(t_data	*data)
{
	int	i;

	i = 0; 
	while (i < 4 && data->textures[i].addr)
	{
		mlx_destroy_image(data->mlx, data->textures[i].addr);
		data->textures[i].addr = NULL;
		i++;
	}
	mlx_destroy_image(data->mlx, data->weapon_texture.img);
	mlx_destroy_image(data->mlx, data->weapon_texture.addr);
	free(data->textures[0].addr);
	free(data->textures[1].addr);
	free(data->textures[2].addr);
	free(data->textures[3].addr);
	free(data->weapon_texture.addr);
	free(data->textures->img);
	free(data->weapon_texture.img);
}

int	exit_game(t_data *data)
{
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	exit(0);
}

void	exit_error(const char *error_msg, int exit_code)
{
	fprintf(stderr, "Error: %s\n", error_msg);
	exit(exit_code);
}
