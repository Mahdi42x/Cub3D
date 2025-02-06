/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:18:31 by mawada            #+#    #+#             */
/*   Updated: 2025/02/06 19:30:31 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

char	**parse_map(int fd)
{
	char	**map;
	char	*line;
	int		rows;
	size_t	len;
	char	**new_map;

	map = NULL;
	line = NULL;
	rows = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (rows == 0 && strspn(line, " \n") == strlen(line))
		{
			free(line);
			continue ;
		}
		len = strlen(line);
		if (len > 0 && line[len - 1] == '\n')
		{
			line[len - 1] = '\0';
		}
		new_map = realloc(map, sizeof(char *) * (rows + 2));
		if (new_map == NULL)
		{
			perror("Error reallocating map");
			exit(EXIT_FAILURE);
		}
		map = new_map;
		map[rows] = strdup(line);
		if (map[rows] == NULL)
		{
			perror("Error duplicating line");
			exit(EXIT_FAILURE);
		}
		rows++;
		map[rows] = NULL;
		free(line);
	}
	return (map);
}

char	**parse_map_from_line(char *first_map_line, int fd, t_data *data)
{
	char		**map;
	char		*line;
	int			rows;
	int			player_found;
	t_parsemap	parsemap;

	map = NULL;
	line = first_map_line;
	rows = 0;
	player_found = 0;
	while (line != NULL)
	{
		parsemap.len = strlen(line);
		if (parsemap.len > 0 && line[parsemap.len - 1] == '\n')
			line[parsemap.len - 1] = '\0';
		parsemap.new_map = realloc(map, sizeof(char *) * (rows + 2));
		if (parsemap.new_map == NULL)
		{
			perror("Error reallocating map");
			exit(EXIT_FAILURE);
		}
		map = parsemap.new_map;
		map[rows] = strdup(line);
		if (map[rows] == NULL)
		{
			perror("Error duplicating line");
			exit(EXIT_FAILURE);
		}
		parsemap.x = 0;
		while (map[rows][parsemap.x])
		{
			parsemap.c = map[rows][parsemap.x];
			if (parsemap.c == 'N' || parsemap.c == 'S'
				|| parsemap.c == 'E' || parsemap.c == 'W')
			{
				data->player.x = parsemap.x + 0.5;
				data->player.y = rows + 0.5;
				set_player_orientation(parsemap.c, &data->player);
				if (data->player.foundplayer != 1)
				{
					printf("Error: To many spawns.\n");
					exit(EXIT_FAILURE);
				}
				player_found = 1;
				map[rows][parsemap.x] = '0';
			}
			parsemap.x++;
		}
		rows++;
		map[rows] = NULL;
		line = get_next_line(fd);
	}
	if (!player_found)
	{
		fprintf(stderr,
			"Error: No player spawn (N, E, S, W) found in the map.\n");
		exit(EXIT_FAILURE);
	}
	return (map);
}
