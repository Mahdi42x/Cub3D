/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 18:30:24 by emkalkan          #+#    #+#             */
/*   Updated: 2025/02/13 17:48:37 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

int	find_starting_point(char **map, int rows, int cols, 
		int *x, int *y)
{
	*y = 0;
	while (*y < rows)
	{
		*x = 0;
		while (*x < cols)
		{
			if (map[*y][*x] == '0' || map[*y][*x] == 'N'
				|| map[*y][*x] == 'E' || map[*y][*x] == 'S'
					|| map[*y][*x] == 'W')
				return (1);
			(*x)++;
		}
		(*y)++;
	}
	return (0);
}

int	is_map_enclosed(char **map)
{
	int		rows;
	int		cols;
	int		x;
	int		y;
	int		result;
	char	**visited;

	if (!check_map_validity(map, &rows, &cols))
		return (0);
	if (!find_starting_point(map, rows, cols, &x, &y))
		return (1);
	visited = allocate_visited(rows, cols);
	result = flood_fill(map, x, y, &(t_flood){rows, cols, visited});
	free_visited(visited, rows);
	return (!result);
}
