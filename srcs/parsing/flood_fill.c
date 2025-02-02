/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:33:36 by mawada            #+#    #+#             */
/*   Updated: 2025/02/02 17:50:44 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

int	has_player(char **map)
{
	int	y;
	int	x;

	if (!map || !map[0])
		return (0);
	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == 'N' || map[y][x] == 'E' || map[y][x] == 'S' || map[y][x] == 'W')
				return (1);
			x++;
		}
		y++;
	}
	return (0);
}

int	flood_fill(char	**map, int x, int y, int rows, int cols, char	**visited)
{
	if (x < 0 || y < 0 || x >= cols
		|| y >= rows || map[y][x] == '1' || visited[y][x])
		return (0);
	if (x == 0 || y == 0 || x == cols - 1 || y == rows - 1)
		return (1);
	visited[y][x] = 1;
	return (flood_fill(map, x + 1, y, rows, cols, visited)
		|| flood_fill(map, x - 1, y, rows, cols, visited)
		|| flood_fill(map, x, y + 1, rows, cols, visited)
		|| flood_fill(map, x, y - 1, rows, cols, visited));
}

void	free_visited(char **visited, int rows)
{
	int	i;

	i = 0;
	while (i < rows)
	{
		free(visited[i]);
		i++;
	}
	free(visited);
}

char	**allocate_visited(int rows, int cols)
{
	char	**visited;
	int		i;

	visited = malloc(rows * sizeof(char *));
	if (!visited)
	{
		perror("Error allocating memory for visited map");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < rows)
	{
		visited[i] = calloc(cols, sizeof(char));
		if (!visited[i])
		{
			perror("Error allocating visited row");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	return (visited);
}

int	check_map_validity(char **map, int *rows, int *cols)
{
	*rows = 0;
	*cols = 0;
	if (!map || !map[0])
	{
		fprintf(stderr, "Error: The map is missing or empty.\n");
		return (0);
	}
	while (map[*rows])
		(*rows)++;
	if (*rows == 0)
	{
		fprintf(stderr, "Error: The map contains no valid data.\n");
		return (0);
	}
	*cols = strlen(map[0]);
	return (1);
}

int	is_map_enclosed(char **map)
{
	int		rows;
	int		cols;
	int		y;
	int		x;
	int		result;

	if (!check_map_validity(map, &rows, &cols))
		return (0);
	y = 0;
	while (y < rows)
	{
		x = 0;
		while (x < cols)
		{
			if (map[y][x] == '0' || map[y][x] == 'N' || map[y][x] == 'E' || map[y][x] == 'S' || map[y][x] == 'W')
			{
				result = flood_fill(map, x, y, rows, cols, allocate_visited(rows, cols));
				free_visited(allocate_visited(rows, cols), rows);
				return (!result);
			}
			x++;
		}
		y++;
	}
	free_visited(allocate_visited(rows, cols), rows);
	return (1);
}
