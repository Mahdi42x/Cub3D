/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:31:10 by mawada            #+#    #+#             */
/*   Updated: 2025/02/26 13:57:43 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	free_mapx(t_data	*data)
{
	int	i;

	if (data->map)
	{
		i = 0;
		while (data->map[i])
		{
			free(data->map[i]);
			data->map[i] = NULL;
			i++;
		}
		free_textures(data, 1);
	}
}

int	exit_x(t_data *data)
{
	int	i;

	if (data->map)
	{
		i = 0;
		while (data->map[i])
		{
			free(data->map[i]);
			data->map[i] = NULL;
			i++;
		}
		free(data->map);
		free_textures(data, 1);
	}
	ft_printf("Bye!\n");
	exit_game(data);
	return (0);
}
