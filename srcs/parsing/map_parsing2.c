/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:10:54 by mawada            #+#    #+#             */
/*   Updated: 2025/02/26 16:21:30 by mawada           ###   ########.fr       */
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
	int *player_found, t_data *data, char *line)
{
	int		x;
	char	c;

	x = 0;
	while (map[rows][x])
	{
		c = map[rows][x];
		if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
		{
			if (*player_found)
			{
				printf("Error: Too many spawns.\n");
				free(line);
				*player_found = 2;
				data->many = 13;
				get_next_line(-1);
				break ;
			}
			data->player.x = x + 0.5;
			data->player.y = rows + 0.5;
			set_player_orientation(c, &data->player);
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
	char	*line;
	int		rows;
	int		len;

	map = NULL;
	line = first_map_line;
	rows = 0;
	while (line != NULL)
	{
		len = strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		map = allocate_and_store_line(map, line, rows);
		if (!map)
			get_next_line(-1);
		handle_player_spawn(map, rows, player_found, data, line);
		rows++;
		map[rows] = NULL;
		data->temp_line = get_next_line(fd);
		if (line != first_map_line && *player_found != 2)
			free(line);
		line = data->temp_line;
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
