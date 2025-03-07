/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emkalkan <emkalkan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 18:30:24 by emkalkan          #+#    #+#             */
/*   Updated: 2025/03/03 18:58:03 by emkalkan         ###   ########.fr       */
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

int	is_map_enclosed(char **map, t_data *data)
{
	int		rows;
	int		cols;

	data->x_ff = malloc(sizeof(int));
	data->y_ff = malloc(sizeof(int));
	if (!data->x_ff || !data->y_ff)
		return (0);
	if (!check_map_validity(map, &rows, &cols))
		return (0);
	*data->x_ff = (int)data->player.x;
	*data->y_ff = (int)data->player.y;
	data->visited = allocate_visited(rows, cols);
	data->result = flood_fill(map, *data->x_ff, *data->y_ff,
			&(t_flood){rows, cols, data->visited});
	if (data->result)
	{
		printf("Error: The map is not enclosed!\n");
		free_visited(data->visited, rows);
		free_x_y_ff(data);
		return (0);
	}
	free_visited(data->visited, rows);
	free_x_y_ff(data);
	return (1);
}
