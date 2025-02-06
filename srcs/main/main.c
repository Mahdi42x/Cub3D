/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 15:56:36 by mawada            #+#    #+#             */
/*   Updated: 2025/02/06 18:39:25 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

int	main(int argc, char	*argv[])
{
	t_data	data;
	int		fd;
	int		i;

	if (argc != 2 || !is_cub_file(argv[1]))
	{
		printf("Usage: %s <map.cub>\n", argv[0]);
		return (1);
	}
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening file");
		return (1);
	}
	data.mlx = mlx_init();
	if (!data.mlx)
	{
		fprintf(stderr, "Error: Failed to initialize MiniLibX.\n");
		return (1);
	}
	init_data(&data);
	parse_cub_file(&data, argv[1]);
	if (!data.map || !data.map[0])
	{
		fprintf(stderr, "Error: The map is missing or empty.\n");
		exit(EXIT_FAILURE);
	}
	if (!is_map_enclosed(data.map))
	{
		fprintf(stderr, "Error: The map is not fully enclosed by walls.\n");
		exit(EXIT_FAILURE);
	}
	print_texture_paths(&data);
	test_all_textures(&data);
	load_weapon_texture(&data, "textures/m4default.xpm");
	data.win = mlx_new_window(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D");
	if (!data.win)
	{
		fprintf(stderr, "Error: Failed to create a window.\n");
		return (1);
	}
	mlx_mouse_hide(data.mlx, data.win);
	mlx_mouse_move(data.mlx, data.win, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	mlx_hook(data.win, 2, 1L << 0, handle_keypress, &data);
	mlx_hook(data.win, 6, 1L << 6, handle_mouse, &data);
	mlx_hook(data.win, 9, 0, handle_focus, &data);
	mlx_hook(data.win, 10, 0, handle_focus, &data);
	mlx_loop_hook(data.mlx, render, &data);
	mlx_loop(data.mlx);
	i = 0;
	while (data.map[i] != NULL)
	{
		free(data.map[i]);
		i++;
	}
	free(data.map);
	return (0);
}
