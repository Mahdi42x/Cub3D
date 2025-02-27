/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:41:16 by mawada            #+#    #+#             */
/*   Updated: 2025/02/26 19:09:56 by mawada           ###   ########.fr       */
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
		fprintf(stderr, "MiniLibX Error: Failed to load ");
		fprintf(stderr, "weapon texture at path: %s\n", path);
		exit(EXIT_FAILURE);
	}
	data->weapon_texture.addr = mlx_get_data_addr(
			data->weapon_texture.img,
			&data->weapon_texture.bpp,
			&data->weapon_texture.line_length,
			&data->weapon_texture.endian);
}

void	free_loadtex(t_data *data, char *line)
{
	printf("Failed to load texture at path: %s\n", line);
	free_textures(data, 0);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	free(line - 3); 
	get_next_line(-1);
	exit(EXIT_FAILURE);
}

void	load_texture(t_data *data, t_texture *texture, char	*line)
{
	char	*newline;

	while (*line && isspace(*line))
		line++;
	newline = strchr(line, '\n');
	//printf("Loading texture at path: %s\n", newline);
	if (newline)
		*newline = '\0';
	if (*line == '\0' || *line == ' ')
	{
		if (line[3] != ' ')
		{
			printf("Failed to load texture at path: %s\n", line);
			free_textures(data, 0);
			// free(line - 1);
			mlx_destroy_display(data->mlx);
			free(data->mlx);
			get_next_line(-1);
			exit(EXIT_FAILURE);
		}
		printf("Error: Missing texture.\n");
		free(line - 3);
		get_next_line(-1);
		exit(EXIT_FAILURE);
	}
	texture->img = mlx_xpm_file_to_image(data->mlx,
			line, &texture->width, &texture->height);
	if (!texture->img)
		free_loadtex(data, line);
	texture->addr = mlx_get_data_addr(texture->img,
			&texture->bpp, &texture->line_length, &texture->endian);
}
