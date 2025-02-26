/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:04:22 by mawada            #+#    #+#             */
/*   Updated: 2025/02/26 18:38:50 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

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

void	free_textures(t_data *data, int weapon)
{
	if (data->mlx)
	{
		if (data->textures[0].img)
			mlx_destroy_image(data->mlx, data->textures[0].img);
		if (data->textures[1].img)
			mlx_destroy_image(data->mlx, data->textures[1].img);
		if (data->textures[2].img)
			mlx_destroy_image(data->mlx, data->textures[2].img);
		if (data->textures[3].img)
			mlx_destroy_image(data->mlx, data->textures[3].img);
		if (data->weapon_texture.img && weapon == 1)
			mlx_destroy_image(data->mlx, data->weapon_texture.img);
	}
	if (data->no_path)
		free(data->no_path);
	if (data->so_path)
		free(data->so_path);
	if (data->we_path)
		free(data->we_path);
	if (data->ea_path)
		free(data->ea_path);
}

void	free_map(char **map, int height)
{
	int	i;

	i = 0;
	while (i < height)
	{
		free(map[i]);
		i++;
	}
	free(map);
}

void	free_maps(t_data *data)
{
	int	i;

	if (data->map)
	{
		i = 0;
		while (data->map[i])
		{
			free(data->map[i]);
			data->map[i] = NULL;
			i++;
		}
	}
}

void	free_visited(char **visited, int rows)
{
	int	i;

	i = 0;
	while (i < rows)
	{
		free(visited[i]);
		i++;
	}
	free(visited);
}
