/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:33:36 by mawada            #+#    #+#             */
/*   Updated: 2025/02/06 17:23:31 by mawada           ###   ########.fr       */
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
			if (map[y][x] == 'N' || map[y][x] == 'E'
				|| map[y][x] == 'S' || map[y][x] == 'W')
				return (1);
			x++;
		}
		y++;
	}
	return (0);
}

int	flood_fill(char **map, int x, int y, t_flood *flood)
{
	if (x < 0 || y < 0 || x >= flood->cols
		|| y >= flood->rows || map[y][x] == '1' || flood->visited[y][x])
		return (0);
	if (x == 0 || y == 0 || x == flood->cols - 1 || y == flood->rows - 1)
		return (1);
	flood->visited[y][x] = 1;
	return (flood_fill(map, x + 1, y, flood)
		|| flood_fill(map, x - 1, y, flood)
		|| flood_fill(map, x, y + 1, flood)
		|| flood_fill(map, x, y - 1, flood));
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
		free_and_exit(visited, "allocating memory for visited map.", 1);
	i = 0;
	while (i < rows)
	{
		visited[i] = calloc(cols, sizeof(char));
		if (!visited[i])
			free_and_exit(visited, "allocating visited row.", 1);
		i++;
	}
	return (visited);
}

int	check_map_validity(char **map, int *rows, int *cols)
{
	*rows = 0;
	*cols = 0;
	if (!map || !map[0])
		free_and_exit(NULL, "The map is missing or empty.", 0);
	while (map[*rows])
		(*rows)++;
	if (*rows == 0)
		free_and_exit(NULL, "The map contains no valid data.", 0);
	*cols = strlen(map[0]);
	return (1);
}
