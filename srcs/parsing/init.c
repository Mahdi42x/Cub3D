/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:42:53 by mawada            #+#    #+#             */
/*   Updated: 2025/02/06 17:54:30 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	set_player_orientation(char direction, t_player *player)
{
	if (direction == 'N')
	{
		player->dir_y = -1;
		player->plane_x = 0.66;
	}
	else if (direction == 'S')
	{
		player->dir_y = 1;
		player->plane_x = -0.66;
	}
	else if (direction == 'E')
	{
		player->dir_x = 1;
		player->plane_y = 0.66;
	}
	else if (direction == 'W')
	{
		player->dir_x = -1;
		player->plane_y = -0.66;
	}
}

void init_data(t_data *data)
{
    data->player.x = 0;
    data->player.y = 0;
    data->player.dir_x = 0;
    data->player.dir_y = 0;
    data->player.plane_x = 0;
    data->player.plane_y = 0;

	data->map_height = 100;
    data->ceiling_color = 0;
    data->floor_color = 0;

}