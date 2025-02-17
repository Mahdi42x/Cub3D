/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emkalkan <emkalkan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:41:16 by mawada            #+#    #+#             */
/*   Updated: 2025/02/17 16:43:36 by emkalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	load_weapon_texture(t_data *data, char	*path)
{
	data->weapon_texture.img = mlx_xpm_file_to_image(data->mlx,
			path, &data->weapon_texture.width, &data->weapon_texture.height);
	if (!data->weapon_texture.img)
	{
		perror("System Error");
		fprintf(stderr,
			"MiniLibX Error: Failed to load weapon texture at path: %s\n", path);
		exit(EXIT_FAILURE);
	}
	data->weapon_texture.addr = mlx_get_data_addr(
			data->weapon_texture.img,
			&data->weapon_texture.bpp,
			&data->weapon_texture.line_length,
			&data->weapon_texture.endian
			);
}

void	load_texture(t_data *data, t_texture *texture, char	*line)
{
	char	*newline;

	while (*line && isspace(*line))
		line++;
	newline = strchr(line, '\n');
	if (newline)
		*newline = '\0';
	// Add this check to ensure line is not empty
	if (*line == '\0')
	{
		fprintf(stderr, "Error: Missing texture path.\n");
		exit(EXIT_FAILURE);
	}
	texture->img = mlx_xpm_file_to_image(data->mlx,
			line, &texture->width, &texture->height);
	if (!texture->img)
	{
		perror("System Error");
		fprintf(stderr,
			"MiniLibX Error: Failed to load texture at path: %s\n", line);
		exit(EXIT_FAILURE);
	}
	texture->addr = mlx_get_data_addr(texture->img,
			&texture->bpp, &texture->line_length, &texture->endian);
}
