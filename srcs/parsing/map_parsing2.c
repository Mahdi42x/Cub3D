/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:10:54 by mawada            #+#    #+#             */
/*   Updated: 2025/02/11 16:22:52 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	free_map(char **map, int rows)
{
	int	i;

	if (!map)
		return ;
	i = 0;
	while (i < rows)
	{
		free(map[i]);
		map[i] = NULL;
		i++;
	}
	free(map);
	map = NULL;
}

char	**parse_map_from_line(char *first_map_line, int fd, t_data *data)
{
	t_parsemap	parsemap;
	char		**map;
	char		*line;
	int			rows;
	int			player_found;
	char 		**temp;
	char 		*temp_line;

	map = NULL;
	line = first_map_line;
	rows = 0;
	player_found = 0;
	while (line != NULL)
	{
		parsemap.len = strlen(line);
		if (parsemap.len > 0 && line[parsemap.len - 1] == '\n')
			line[parsemap.len - 1] = '\0';
		temp = realloc(map, sizeof(char *) * (rows + 2));
		if (!temp)
		{
			perror("Error reallocating map");
			free_map(map, rows);
			exit(EXIT_FAILURE);
		}
		map = temp;
		map[rows] = strdup(line);
		if (!map[rows])
		{
			perror("Error duplicating line");
			free_map(map, rows);
			exit(EXIT_FAILURE);
		}
		parsemap.x = 0;
		while (map[rows][parsemap.x])
		{
			parsemap.c = map[rows][parsemap.x];
			if (parsemap.c == 'N' || parsemap.c == 'S'
				|| parsemap.c == 'E' || parsemap.c == 'W')
			{
				if (player_found)
				{
					fprintf(stderr, "Error: Too many spawns.\n");
					free_map(map, rows + 1);
					exit(EXIT_FAILURE);
				}
				data->player.x = parsemap.x + 0.5;
				data->player.y = rows + 0.5;
				set_player_orientation(parsemap.c, &data->player);
				player_found = 1;
				map[rows][parsemap.x] = '0';
			}
			parsemap.x++;
		}
		rows++;
		map[rows] = NULL;
		temp_line = get_next_line(fd);
		if (line != first_map_line)
			free(line);
		line = temp_line;
	}
	if (!player_found)
	{
		fprintf(stderr,
			"Error: No player spawn (N, E, S, W) found in the map.\n");
		free_map(map, rows);
		exit(EXIT_FAILURE);
	}
	return (map);
}
