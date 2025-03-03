/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emkalkan <emkalkan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:33:36 by mawada            #+#    #+#             */
/*   Updated: 2025/03/03 18:57:58 by emkalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

int	flood_fill(char **map, int x, int y, t_flood *flood)
{
	if (x < 0 || y < 0 || x >= flood->cols - 1 || y >= flood->rows - 1)
	{
		printf("Error: Map is not enclosed! Open boundary at (%d, %d)\n", x, y);
		return (1);
	}
	if (x < 0 || y < 0 || x >= flood->cols
		|| y >= flood->rows || map[y][x] == '1' || flood->visited[y][x])
		return (0);
	flood->visited[y][x] = 1;
	return (flood_fill(map, x + 1, y, flood)
		|| flood_fill(map, x - 1, y, flood)
		|| flood_fill(map, x, y + 1, flood)
		|| flood_fill(map, x, y - 1, flood));
}

char	**allocate_visited(int rows, int cols)
{
	char	**visited;
	int		i;

	visited = malloc(rows * sizeof(char *));
	if (!visited)
	{
		printf("The map is missing or empty.");
		free(visited);
		exit (0);
	}
	i = 0;
	while (i < rows)
	{
		visited[i] = calloc(cols, sizeof(char));
		if (!visited[i])
			free(visited);
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
		printf("The map is missing or empty.");
		exit (0);
	}
	while (map[*rows])
		(*rows)++;
	if (*rows == 0)
	{
		printf("The map contains no valid data.");
		exit (0);
	}
	*cols = strlen(map[0]);
	return (1);
}
