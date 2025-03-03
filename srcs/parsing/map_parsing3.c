/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:47:39 by mawada            #+#    #+#             */
/*   Updated: 2025/03/03 17:36:18 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

char	**parse_map_from_line(char *first_map_line,
		int fd, t_data *data)
{
	int		player_found;
	char	**map;

	player_found = 0;
	map = read_map_lines(first_map_line, fd, data, &player_found);
	validate_map_characters(map, data);
	if (player_found == 0 || player_found == 2 || data->many2 == 13)
	{
		if (data->many != 13)
			printf("Error: No player spawn (N, E, S, W) found in the map.\n");
		if (map)
			parse_map_line_free_map_helper(map);
		free_textures(data, 0);
		mlx_destroy_display(data->mlx);
		free(data->map);
		free(data->mlx);
		free(first_map_line);
		get_next_line(-1);
		exit(EXIT_FAILURE);
	}
	return (map);
}

void	handle_line(t_data *data, char *line, int *has_fc)
{
	if (strncmp(line, "NO ", 3) == 0 || strncmp(line, "SO ", 3) == 0
		|| strncmp(line, "WE ", 3) == 0 || strncmp(line, "EA ", 3) == 0)
		handle_texture(line, data);
	else if (strncmp(line, "F ", 2) == 0 || strncmp(line, "C ", 2) == 0)
	{
		handle_color(line, data);
		*has_fc = 1;
	}
}

int	process_map_part(t_data *data, char *line, int fd, int has_fc)
{
	if (!has_fc)
	{
		printf("Error: Missing 'F' or 'C' in configuration.\n");
		free(line);
		close(fd);
		get_next_line(-1);
		return (0);
	}
	parse_maps(data, line, fd);
	if (!is_map_enclosed(data->map, data))
	{
		printf("Error: The map is not enclosed by walls.\n");
		return (0);
	}
	return (1);
}

void	parse_cub_file_loop(t_data *data, char *line, int fd)
{
	int	has_fc;

	has_fc = 0;
	while (line)
	{
		handle_line(data, line, &has_fc);
		if (*line == '1' || *line == '0' || *line == ' ')
		{
			if (!process_map_part(data, line, fd, has_fc))
				break ;
			break ;
		}
		free(line);
		line = get_next_line(fd);
	}
}
