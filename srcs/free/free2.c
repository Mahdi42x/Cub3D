/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:31:10 by mawada            #+#    #+#             */
/*   Updated: 2025/02/13 16:31:28 by mawada           ###   ########.fr       */
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
		free_textures(data);
	}
}

int	exit_x(t_data *data)
{
	ft_printf("Bye!\n");
	free_mapx(data);
	exit_game(data);
	return (0);
}