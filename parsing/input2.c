/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 14:42:48 by mawada            #+#    #+#             */
/*   Updated: 2025/02/21 15:18:47 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

int	handle_keypress(int key, t_data *data)
{
	int	i;

	if (key == KEY_ESC)
	{
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
		exit_game(data);
	}
	if (key == KEY_W || key == KEY_A || key == KEY_S || key == KEY_D
		|| key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT
		|| key == KEY_RIGHT)
	{
		move_player(data, key);
	}
	return (0);
}
