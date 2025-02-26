/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:35:41 by mawada            #+#    #+#             */
/*   Updated: 2025/02/26 18:28:13 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	handle_texture(char *line, t_data *data)
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

void printMap(char **map)
{
	if (!map) return;
	size_t i = 0;
	while (map[i] != NULL) {
		size_t j = 0;
		while (map[i][j] != '\0') {
			write(1, &map[i][j], 1); // Zeichenweise ausgeben
			++j;
		}
		write(1, "\n", 1); // Neue Zeile
		++i;
	}
}

void	parse_cub_file_helper(t_data *data)
{
	//printMap(data->map);
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
