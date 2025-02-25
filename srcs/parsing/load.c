/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:41:16 by mawada            #+#    #+#             */
/*   Updated: 2025/02/24 16:33:37 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	load_weapon_texture(t_data *data, char *path)
{
	data->weapon_texture.img = mlx_xpm_file_to_image(data->mlx,
			path, &data->weapon_texture.width, &data->weapon_texture.height);
	if (!data->weapon_texture.img)
	{
		perror("System Error");
		fprintf(stderr, "MiniLibX Error: Failed to load weapon texture at path: %s\n", path);
		exit(EXIT_FAILURE);
	}

	data->weapon_texture.addr = mlx_get_data_addr(
			data->weapon_texture.img,
			&data->weapon_texture.bpp,
			&data->weapon_texture.line_length,
			&data->weapon_texture.endian);
}


void	load_texture(t_data *data, t_texture *texture, char	*line)
{
	char	*newline;

	while (*line && isspace(*line))
		line++;
	newline = strchr(line, '\n');
	if (newline)
		*newline = '\0';
	if (*line == '\0')
	{
		fprintf(stderr, "Error: Missing texture path.\n");
		free(line - 3); 
		get_next_line(-1);
		exit(EXIT_FAILURE);
	}
	texture->img = mlx_xpm_file_to_image(data->mlx,
			line, &texture->width, &texture->height);
	if (!texture->img)
	{
		perror("System Error");
		fprintf(stderr,
			"MiniLibX Error: Failed to load texture at path: %s\n", line);
		if (data->textures[0].img)
			mlx_destroy_image(data->mlx, data->textures[0].img);
		if (data->textures[1].img)
			mlx_destroy_image(data->mlx, data->textures[1].img);
		if (data->textures[2].img)
			mlx_destroy_image(data->mlx, data->textures[2].img);
		if (data->textures[3].img)
			mlx_destroy_image(data->mlx, data->textures[3].img);
		free(data->no_path);
		free(data->so_path);
		free(data->we_path);
		free(data->ea_path);
		mlx_destroy_display(data->mlx);
		free(data->mlx);
		free(line - 3); 
		get_next_line(-1);
		exit(EXIT_FAILURE);
	}
	texture->addr = mlx_get_data_addr(texture->img,
			&texture->bpp, &texture->line_length, &texture->endian);
}
