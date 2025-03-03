/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emkalkan <emkalkan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:10:54 by mawada            #+#    #+#             */
/*   Updated: 2025/03/03 19:56:38 by emkalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	validate_map_characters(char **map, t_data *data)
{
	int		y;
	int		x;
	char	c;

	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			c = map[y][x];
			if (c != 'N' && c != 'E' && c != 'S' && c != 'W'
				&& c != '1' && c != '0' && c != ' ')
			{
				printf("Error: Invalid character '%c' found in map.\n", c);
				data->many2 = 13;
				get_next_line(-1);
				data->map = NULL;
			}
			x++;
		}
		y++;
	}
}

char	**allocate_and_store_line(char **map, char *line, int rows)
{
	char	**temp;

	temp = realloc(map, sizeof(char *) * (rows + 2));
	if (!temp)
	{
		perror("Error reallocating map");
		free_map(map, rows);
		free(line);
		free(temp);
		get_next_line(-1);
		exit(EXIT_FAILURE);
	}
	map = temp;
	map[rows] = strdup(line);
	if (!map[rows])
	{
		perror("Error duplicating line");
		free_map(map, rows);
		free(map[rows]);
		free(map);
		free(line); 
		get_next_line(-1);
		exit(EXIT_FAILURE);
	}
	return (map);
}

void	handle_player_spawn(char **map, int rows,
	int *player_found, t_data *data)
{
	int		x;

	x = 0;
	while (map[rows][x])
	{
		if (map[rows][x] == 'N' || map[rows][x] == 'S'
			|| map[rows][x] == 'E' || map[rows][x] == 'W')
		{
			if (*player_found)
			{
				free(data->line);
				*player_found = 2;
				data->many = 13;
				get_next_line(-1);
				break ;
			}
			data->player.x = x + 0.5;
			data->player.y = rows + 0.5;
			set_player_orientation(map[rows][x], &data->player);
			*player_found = 1;
			map[rows][x] = '0';
		}
		x++;
	}
}

char	**read_map_lines(char *first_map_line, int fd,
	t_data *data, int *player_found)
{
	char	**map;
	int		rows;
	int		len;

	map = NULL;
	data->line = first_map_line;
	rows = 0;
	while (data->line != NULL)
	{
		len = strlen(data->line);
		if (len > 0 && data->line[len - 1] == '\n')
			data->line[len - 1] = '\0';
		map = allocate_and_store_line(map, data->line, rows);
		if (!map)
			get_next_line(-1);
		handle_player_spawn(map, rows, player_found, data);
		rows++;
		map[rows] = NULL;
		data->temp_line = get_next_line(fd);
		if (data->line != first_map_line && *player_found != 2)
			free(data->line);
		data->line = data->temp_line;
	}
	return (map);
}

void	parse_map_line_free_map_helper(char **map)
{
	free(map[0]);
	free(map[1]);
	free(map[2]);
	free(map);
}
