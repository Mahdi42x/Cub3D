/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_parsing3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:47:13 by mawada            #+#    #+#             */
/*   Updated: 2025/03/03 16:47:20 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	parse_cub_file_helper(t_data *data)
{
	if (!data->map || !data->map[0])
	{
		printf("Error: Map data is missing or empty in");
		printf(" the .cub file.\n");
		mlx_destroy_image(data->mlx, data->textures[0].img);
		mlx_destroy_image(data->mlx, data->textures[1].img);
		mlx_destroy_image(data->mlx, data->textures[2].img);
		mlx_destroy_image(data->mlx, data->textures[3].img);
		free(data->no_path);
		free(data->so_path);
		free(data->we_path);
		free(data->ea_path);
		mlx_destroy_display(data->mlx);
		free(data->mlx);
		free(data->map);
		exit(1);
	}
	set_map_width(data);
}
