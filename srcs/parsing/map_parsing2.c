/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:10:54 by mawada            #+#    #+#             */
/*   Updated: 2025/02/24 16:42:01 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

char	**allocate_and_store_line(char **map, char *line, int rows)
{
	char	**temp;

	temp = realloc(map, sizeof(char *) * (rows + 2));
	if (!temp)
	{
		perror("Error reallocating map");
		free_map(map, rows);
		printf("FREE:   %p", (void *)(line));
		free(line); 
		exit(EXIT_FAILURE);
	}
	map = temp;
	map[rows] = strdup(line);
	if (!map[rows])
	{
		perror("Error duplicating line");
		free(map);
		free_map(map, rows);
		printf("FREE:   %p", (void *)(line));
		free(line); 
		exit(EXIT_FAILURE);
	}
	return (map);
}

void	handle_player_spawn(char **map, int rows,
	int *player_found, t_data *data, char *line) 
{
	int		x;
	char	c;

	x = 0;
	while (map[rows][x])
	{
		c = map[rows][x];
		if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
		{
			if (*player_found)
			{
				fprintf(stderr, "Error: Too many spawns.\n");
				free_map(map, rows + 1);
				printf("FREE:   %p", (void *)(line));
				free(line); 
				exit(EXIT_FAILURE);
			}
			data->player.x = x + 0.5;
			data->player.y = rows + 0.5;
			set_player_orientation(c, &data->player);
			*player_found = 1;
			map[rows][x] = '0';
		}
		x++;
	}
}

char	**read_map_lines(char *first_map_line, int fd,
	t_data *data, int *player_found)
{
	char	**map;
	char	*line;
	int		rows;
	int		len;

	map = NULL;
	line = first_map_line;
	rows = 0;
	while (line != NULL)
	{
		len = strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		map = allocate_and_store_line(map, line, rows);
		handle_player_spawn(map, rows, player_found, data, line); 
		rows++;
		map[rows] = NULL;
		data->temp_line = get_next_line(fd);
		if (line != first_map_line)
			free(line);
		line = data->temp_line;
	}
	return (map);
}

char	**parse_map_from_line(char *first_map_line, int fd, t_data *data)
{
	int		player_found;
	char	**map;

	player_found = 0;
	map = read_map_lines(first_map_line, fd, data, &player_found);
	if (!player_found)
	{
		fprintf(stderr, "Error: No player spawn (N, E, S, W) found in the map.\n");
		if (map)
		{
			free(map[0]);
			free(map[1]);
			free(map[2]);
			free(map);
		}
		mlx_destroy_image(data->mlx, data->textures[0].img);
		mlx_destroy_image(data->mlx, data->textures[1].img);
		mlx_destroy_image(data->mlx, data->textures[2].img);
		mlx_destroy_image(data->mlx, data->textures[3].img);
		free(data->no_path);
		free(data->so_path);
		free(data->we_path);
		free(data->ea_path);
		mlx_destroy_display(data->mlx);
		free(data->map);
		free(data->mlx);
		free(first_map_line);
		exit(EXIT_FAILURE);
	}
	return (map);
}

// char	**parse_map_from_line(char *first_map_line, int fd, t_data *data)
// {
// 	int		player_found;
// 	char	**map;

// 	player_found = 0;
// 	map = read_map_lines(first_map_line, fd, data, &player_found);
// 	if (!player_found)
// 	{
// 		fprintf(stderr,
// 			"Error: No player spawn (N, E, S, W) found in the map.\n");
// 		mlx_destroy_image(data->mlx, data->textures[0].img);
// 		mlx_destroy_image(data->mlx, data->textures[1].img);
// 		mlx_destroy_image(data->mlx, data->textures[2].img);
// 		mlx_destroy_image(data->mlx, data->textures[3].img);
// 		free(data->no_path);
// 		free(data->so_path);
// 		free(data->we_path);
// 		free(data->ea_path);
// 		//free(map);
// 		mlx_destroy_display(data->mlx);
// 		free(data->map);
// 		free(data->mlx);
// 		exit(EXIT_FAILURE);
// 	}
// 	return (map);
// }

// char	**parse_map_from_line(char *first_map_line, int fd, t_data *data)
// {
// 	t_parsemap	parsemap;
// 	char		**map;
// 	char		*line;
// 	int			rows;
// 	int			player_found;

// 	map = NULL;
// 	line = first_map_line;
// 	rows = 0;
// 	player_found = 0;
// 	while (line != NULL)
// 	{
// 		parsemap.len = strlen(line);
// 		if (parsemap.len > 0 && line[parsemap.len - 1] == '\n')
// 			line[parsemap.len - 1] = '\0';
// 		data->temp = realloc(map, sizeof(char *) * (rows + 2));
// 		if (!data->temp)
// 		{
// 			perror("Error reallocating map");
// 			free_map(map, rows);
// 			exit(EXIT_FAILURE);
// 		}
// 		map = data->temp;
// 		map[rows] = strdup(line);
// 		if (!map[rows])
// 		{
// 			perror("Error duplicating line");
// 			free_map(map, rows);
// 			exit(EXIT_FAILURE);
// 		}
// 		parsemap.x = 0;
// 		while (map[rows][parsemap.x])
// 		{
// 			parsemap.c = map[rows][parsemap.x];
// 			if (parsemap.c == 'N' || parsemap.c == 'S'
// 				|| parsemap.c == 'E' || parsemap.c == 'W')
// 			{
// 				if (player_found)
// 				{
// 					fprintf(stderr, "Error: Too many spawns.\n");
// 					free_map(map, rows + 1);
// 					exit(EXIT_FAILURE);
// 				}
// 				data->player.x = parsemap.x + 0.5;
// 				data->player.y = rows + 0.5;
// 				set_player_orientation(parsemap.c, &data->player);
// 				player_found = 1;
// 				map[rows][parsemap.x] = '0';
// 			}
// 			parsemap.x++;
// 		}
// 		rows++;
// 		map[rows] = NULL;
// 		data->temp_line = get_next_line(fd);
// 		if (line != first_map_line)
// 			free(line);
// 		line = data->temp_line;
// 	}
// 	if (!player_found)
// 	{
// 		fprintf(stderr,
// 			"Error: No player spawn (N, E, S, W) found in the map.\n");
// 		free_map(map, rows);
// 		exit(EXIT_FAILURE);
// 	}
// 	return (map);
// }
