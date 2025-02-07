
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emkalkan <emkalkan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 15:56:36 by mawada            #+#    #+#             */
/*   Updated: 2025/02/07 16:43:29 by emkalkan         ###   ########.fr       */
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
		exit(1);
	}
	data->mlx = mlx_init();
	if (!data->mlx)
	{
		fprintf(stderr, "Error: Failed to initialize MiniLibX.\n");
		exit(1);
	}
}

void	load_map_and_textures(t_data *data, char *filename)
{
	init_data(data);
	parse_cub_file(data, filename);
	if (!data->map || !data->map[0])
	{
		fprintf(stderr, "Error: The map is missing or empty.\n");
		exit(1);
	}
	if (!is_map_enclosed(data->map))
	{
		fprintf(stderr, "Error: The map is not fully enclosed by walls.\n");
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
		exit(1);
	}
	mlx_mouse_hide(data->mlx, data->win);
	mlx_mouse_move(data->mlx, data->win, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	mlx_hook(data->win, 2, 1L << 0, handle_keypress, data);
	mlx_hook(data->win, 6, 1L << 6, handle_mouse, data);
	mlx_hook(data->win, 9, 0, handle_focus, data);
	mlx_hook(data->win, 10, 0, handle_focus, data);
	mlx_loop_hook(data->mlx, render, data);
}

void	cleanup_and_exit(t_data *data)
{
	int	i;

	i = 0;
	freeimg(data);
	while (data->map[i] != NULL)
	{
		free(data->map[i]);
		i++;
	}
	free(data->map);
}

int	main(int argc, char *argv[])
{
	t_data	data;
	int		fd;

	check_args_and_init(argc, argv, &fd, &data);
	load_map_and_textures(&data, argv[1]);
	setup_window_and_hooks(&data);
	mlx_loop(data.mlx);
	cleanup_and_exit(&data);
	return (0);
}

// int	main(int argc, char	*argv[])
// {
// 	t_data	data;
// 	int		fd;
// 	int		i;

// 	if (argc != 2 || !is_cub_file(argv[1]))
// 	{
// 		printf("Usage: %s <map.cub>\n", argv[0]);
// 		return (1);
// 	}
// 	fd = open(argv[1], O_RDONLY);
// 	if (fd == -1)
// 	{
// 		perror("Error opening file");
// 		return (1);
// 	}
// 	data.mlx = mlx_init();
// 	if (!data.mlx)
// 	{
// 		fprintf(stderr, "Error: Failed to initialize MiniLibX.\n");
// 		return (1);
// 	}
// 	init_data(&data);
// 	parse_cub_file(&data, argv[1]);
// 	if (!data.map || !data.map[0])
// 	{
// 		fprintf(stderr, "Error: The map is missing or empty.\n");
// 		exit(EXIT_FAILURE);
// 	}
// 	if (!is_map_enclosed(data.map))
// 	{
// 		fprintf(stderr, "Error: The map is not fully enclosed by walls.\n");
// 		exit(EXIT_FAILURE);
// 	}
// 	print_texture_paths(&data);
// 	test_all_textures(&data);
// 	load_weapon_texture(&data, "textures/m4default.xpm");
// 	data.win = mlx_new_window(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D");
// 	if (!data.win)
// 	{
// 		fprintf(stderr, "Error: Failed to create a window.\n");
// 		return (1);
// 	}
// 	mlx_mouse_hide(data.mlx, data.win);
// 	mlx_mouse_move(data.mlx, data.win, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
// 	mlx_hook(data.win, 2, 1L << 0, handle_keypress, &data);
// 	mlx_hook(data.win, 6, 1L << 6, handle_mouse, &data);
// 	mlx_hook(data.win, 9, 0, handle_focus, &data);
// 	mlx_hook(data.win, 10, 0, handle_focus, &data);
// 	mlx_loop_hook(data.mlx, render, &data);
// 	mlx_loop(data.mlx);
// 	i = 0;
// 	while (data.map[i] != NULL)
// 	{
// 		free(data.map[i]);
// 		i++;
// 	}
// 	free(data.map);
// 	return (0);
// }
