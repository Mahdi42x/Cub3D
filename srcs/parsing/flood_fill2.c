/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 18:30:24 by emkalkan          #+#    #+#             */
/*   Updated: 2025/02/21 18:17:22 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

int	find_starting_point(char **map, int rows, int cols, t_data *data)
{
	*data->y_ff = 0;
	while (*data->y_ff < rows)
	{
		*data->x_ff = 0;
		while (*data->x_ff < cols)
		{
			if (map[*data->y_ff][*data->x_ff] == '0' ||
				map[*data->y_ff][*data->x_ff] == 'N' ||
				map[*data->y_ff][*data->x_ff] == 'E' ||
				map[*data->y_ff][*data->x_ff] == 'S' ||
				map[*data->y_ff][*data->x_ff] == 'W')
			{
				return (1);
			}
			(*data->x_ff)++;
		}
		(*data->y_ff)++;
	}
	return (0);
}

int	free_x_y_ff(t_data *data)
{
	free(data->x_ff);
	free(data->y_ff);
	return (0);
}

int	is_map_enclosed(char **map)
{
    t_data	data;
    int		rows;
    int		cols;

    data.x_ff = malloc(sizeof(int));
    data.y_ff = malloc(sizeof(int));
    if (!data.x_ff || !data.y_ff)
        return (0);
    if (!check_map_validity(map, &rows, &cols))
        return (0);
    if (!find_starting_point(map, rows, cols, &data))
    {
		free_x_y_ff(&data);
        //free(data.x_ff);
        //free(data.y_ff);
        return (1);
    }
    data.visited = allocate_visited(rows, cols);
    data.result = flood_fill(map, *data.x_ff, *data.y_ff,
            &(t_flood){rows, cols, data.visited});
    free_visited(data.visited, rows);
	free_x_y_ff(&data);
	//free(data.x_ff);
	//free(data.y_ff);
    return (!data.result);  // If result is 1, map is open -> return false
}


// int	find_starting_point(char **map, int rows, int cols, 
// 		int *x, int *y)
// {
// 	*y = 0;
// 	while (*y < rows)
// 	{
// 		*x = 0;
// 		while (*x < cols)
// 		{
// 			if (map[*y][*x] == '0' || map[*y][*x] == 'N'
// 				|| map[*y][*x] == 'E' || map[*y][*x] == 'S'
// 					|| map[*y][*x] == 'W')
// 				return (1);
// 			(*x)++;
// 		}
// 		(*y)++;
// 	}
// 	return (0);
// }

// int	is_map_enclosed(char **map)
// {
// 	t_data	data;
// 	int		rows;
// 	int		cols;
// 	int		x;
// 	int		y;

// 	if (!check_map_validity(map, &rows, &cols))
// 		return (0);
// 	if (!find_starting_point(map, rows, cols, &x, &y))
// 		return (1);
// 	data.visited = allocate_visited(rows, cols);
// 	data.result = flood_fill(map, x, y, &(t_flood){rows, cols, data.visited});
// 	free_visited(data.visited, rows);
// 	return (!data.result);
// }
