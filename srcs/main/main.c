/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 15:56:36 by mawada            #+#    #+#             */
/*   Updated: 2025/02/26 18:32:28 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	check_args_and_init(int argc, char *argv[], int *fd, t_data *data)
{
	if (argc != 2 || !is_cub_file(argv[1]))
	{
		printf("Usage: %s <map.cub>\n", argv[0]);
		exit(1);
	}
	*fd = open(argv[1], O_RDONLY);
	if (*fd == -1)
	{
		perror("Error opening file");
		free_maps(data);
		free(data->map);
		free_textures(data, 1);
		exit_game(data);
		exit(1);
	}
	data->mlx = mlx_init();
	if (!data->mlx)
	{
		fprintf(stderr, "Error: Failed to initialize MiniLibX.\n");
		free_maps(data);
		free(data->map);
		free_textures(data, 1);
		exit_game(data);
		exit(1);
	}
}

void	load_map_and_textures(t_data *data, char *filename)
{
	init_data(data);
	parse_cub_file(data, filename);
	if (!is_map_enclosed(data->map))
	{
		fprintf(stderr, "Error: The map is not fully enclosed by walls.\n");
		free_textures(data, 0);
		free_maps(data);
		free(data->map);
		mlx_destroy_display(data->mlx);
		free(data->mlx);
		get_next_line(-1);
		exit(1);
	}
	print_texture_paths(data);
	test_all_textures(data);
	load_weapon_texture(data, "textures/m4default.xpm");
}

void	setup_window_and_hooks(t_data *data)
{
	data->win = mlx_new_window(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D");
	if (!data->win)
	{
		fprintf(stderr, "Error: Failed to create a window.\n");
		free_maps(data);
		free(data->map);
		free_textures(data, 1);
		exit_game(data);
		exit(1);
	}
	mlx_mouse_move(data->mlx, data->win, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	mlx_hook(data->win, 2, 1L << 0, handle_keypress, data);
	mlx_hook(data->win, 6, 1L << 6, handle_mouse, data);
	mlx_hook(data->win, 9, 0, handle_focus, data);
	mlx_hook(data->win, 10, 0, handle_focus, data);
	mlx_hook(data->win, 17, 0, exit_x, data);
	mlx_loop_hook(data->mlx, render, data);
}

int	main(int argc, char *argv[])
{
	t_data	data;
	int		fd;

	check_args_and_init(argc, argv, &fd, &data);
	load_map_and_textures(&data, argv[1]);
	setup_window_and_hooks(&data);
	mlx_loop(data.mlx);
	return (0);
}
