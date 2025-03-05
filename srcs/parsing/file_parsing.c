/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:35:41 by mawada            #+#    #+#             */
/*   Updated: 2025/03/05 11:54:31 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	handle_texture_cases(t_data *data, char *line)
{
	if (strncmp(line, "NO ", 3) == 0)
	{
		data->no_path = ft_strtrim(line + 3, " \n\t");
		load_texture(data, &data->textures[3], line + 3);
	}
	else if (strncmp(line, "SO ", 3) == 0)
	{
		data->so_path = ft_strtrim(line + 3, " \n\t");
		load_texture(data, &data->textures[2], line + 3);
	}
	else if (strncmp(line, "WE ", 3) == 0)
	{
		data->we_path = ft_strtrim(line + 3, " \n\t");
		load_texture(data, &data->textures[1], line + 3);
	}
	else if (strncmp(line, "EA ", 3) == 0)
	{
		data->ea_path = ft_strtrim(line + 3, " \n\t");
		load_texture(data, &data->textures[0], line + 3);
	}
}

void	handle_texture(char *line, t_data *data)
{
	if (line[3] == '\0' || isspace(line[3])) 
	{
		printf("Failed to load texture at path: %s\n", line);
		free(line);
		free_textures(data, 0);
		mlx_destroy_display(data->mlx);
		free(data->mlx);
		get_next_line(-1);
		exit(EXIT_FAILURE);
	}
	handle_texture_cases(data, line);
}

void	handle_color(char *line, t_data *data)
{
	if (strncmp(line, "F ", 2) == 0)
		data->floor_color = parse_color(line + 2, 0, data);
	else if (strncmp(line, "C ", 2) == 0)
		data->ceiling_color = parse_color(line + 2, 1, data);
}

void	parse_maps(t_data *data, char *line, int fd)
{
	data->map = parse_map_from_line(line, fd, data);
	free(line);
}

void	set_map_width(t_data *data)
{
	int	i;
	int	len;

	data->map_width = 0;
	i = 0;
	while (data->map[i])
	{
		len = strlen(data->map[i]);
		if (len > data->map_width)
			data->map_width = len;
		i++;
	}
}
