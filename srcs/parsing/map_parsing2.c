/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emkalkan <emkalkan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:10:54 by mawada            #+#    #+#             */
/*   Updated: 2025/02/18 17:42:26 by emkalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	free_map(char **map, int rows)
{
	int	i;

	if (!map)
		return ;
	i = 0;
	while (i < rows)
	{
		free(map[i]);
		map[i] = NULL;
		i++;
	}
	free(map);
	map = NULL;
}

char	**parse_map_from_line(char *first_map_line, int fd, t_data *data)
{
	t_parsemap	parsemap;
	char		**map;
	char		*line;
	int			rows;
	int			player_found;

	map = NULL;
	line = first_map_line;
	rows = 0;
	player_found = 0;
	while (line != NULL)
	{
		parsemap.len = strlen(line);
		if (parsemap.len > 0 && line[parsemap.len - 1] == '\n')
			line[parsemap.len - 1] = '\0';
		data->temp = realloc(map, sizeof(char *) * (rows + 2));
		if (!data->temp)
		{
			perror("Error reallocating map");
			free_map(map, rows);
			exit(EXIT_FAILURE);
		}
		map = data->temp;
		map[rows] = strdup(line);
		if (!map[rows])
		{
			perror("Error duplicating line");
			free_map(map, rows);
			exit(EXIT_FAILURE);
		}
		parsemap.x = 0;
		while (map[rows][parsemap.x])
		{
			parsemap.c = map[rows][parsemap.x];
			if (parsemap.c == 'N' || parsemap.c == 'S'
				|| parsemap.c == 'E' || parsemap.c == 'W')
			{
				if (player_found)
				{
					fprintf(stderr, "Error: Too many spawns.\n");
					free_map(map, rows + 1);
					exit(EXIT_FAILURE);
				}
				data->player.x = parsemap.x + 0.5;
				data->player.y = rows + 0.5;
				set_player_orientation(parsemap.c, &data->player);
				player_found = 1;
				map[rows][parsemap.x] = '0';
			}
			parsemap.x++;
		}
		rows++;
		map[rows] = NULL;
		data->temp_line = get_next_line(fd);
		if (line != first_map_line)
			free(line);
		line = data->temp_line;
	}
	if (!player_found)
	{
		fprintf(stderr,
			"Error: No player spawn (N, E, S, W) found in the map.\n");
		free_map(map, rows);
		exit(EXIT_FAILURE);
	}
	return (map);
}











// void handle_player_spawn(t_parsemap *parsemap, t_data *data, int rows, int *player_found, char **map)
// {
// 	if (*player_found) {
// 		fprintf(stderr, "Error: Too many spawns.\n");
// 		free_map(map, rows + 1);
// 		exit(EXIT_FAILURE);
// 	}
// 	data->player.x = parsemap->x + 0.5;
// 	data->player.y = rows + 0.5;
// 	set_player_orientation(parsemap->c, &data->player);
// 	*player_found = 1;
// 	map[rows][parsemap->x] = '0';
// }

// void process_character(t_parsemap *parsemap, t_data *data, int rows, int *player_found, char **map) {
// 	parsemap->c = map[rows][parsemap->x];
// 	if (parsemap->c == 'N' || parsemap->c == 'S' || parsemap->c == 'E' || parsemap->c == 'W') {
// 		handle_player_spawn(parsemap, data, rows, player_found, map);
// 	}
// 	parsemap->x++;
// }

// void process_line_content(t_data *line_data) {
// 	line_data->parsemap->x = 0;
// 	while ((*line_data->map)[*line_data->rows][line_data->parsemap->x]) {
// 		process_character(line_data->parsemap, line_data->data, *line_data->rows, line_data->player_found, *line_data->map);
// 	}
// }

// void allocate_map_memory(t_data *data, int rows, char ***map) {
// 	data->temp = realloc(*map, sizeof(char *) * (rows + 2));
// 	if (!data->temp) {
// 		perror("Error reallocating map");
// 		free_map(*map, rows);
// 		exit(EXIT_FAILURE);
// 	}
// 	*map = data->temp;
// }

// void duplicate_line(char *line, int rows, char **map) {
// 	map[rows] = strdup(line);
// 	if (!map[rows]) {
// 		perror("Error duplicating line");
// 		free_map(map, rows);
// 		exit(EXIT_FAILURE);
// 	}
// }

// void process_line(t_line_data *line_data) {
// 	line_data->parsemap->len = strlen(line_data->line);
// 	if (line_data->parsemap->len > 0 && line_data->line[line_data->parsemap->len - 1] == '\n')
// 		line_data->line[line_data->parsemap->len - 1] = '\0';

// 	allocate_map_memory(line_data->data, *line_data->rows, line_data->map);
// 	duplicate_line(line_data->line, *line_data->rows, *line_data->map);
// 	process_line_content(line_data);

// 	(*line_data->rows)++;
// 	(*line_data->map)[*line_data->rows] = NULL;
// }

// void initialize_line_data(t_line_data *line_data, char *line, t_parsemap *parsemap, t_data *data, int *rows, int *player_found, char ***map) {
// 	line_data->line = line;
// 	line_data->parsemap = parsemap;
// 	line_data->data = data;
// 	line_data->rows = rows;
// 	line_data->player_found = player_found;
// 	line_data->map = map;
// }

// void cleanup_and_exit(char **map, int rows, const char *error_message) {
// 	fprintf(stderr, "%s\n", error_message);
// 	free_map(map, rows);
// 	exit(EXIT_FAILURE);
// }

// char **parse_map_from_line(char *first_map_line, int fd, t_data *data) {
// 	t_parsemap	parsemap;
// 	char		**map;
// 	char		*line;
// 	int			rows;
// 	int			player_found;

// 	map = NULL;
// 	line = first_map_line;
// 	rows = 0;
// 	player_found = 0;
// 	t_line_data line_data;
// 	initialize_line_data(&line_data, line, &parsemap, data, &rows, &player_found, &map);
// 	while (line != NULL) {
// 		process_line(&line_data);
// 		data->temp_line = get_next_line(fd);
// 		if (line != first_map_line)
// 			free(line);
// 		line = data->temp_line;
// 		line_data.line = line;
// 	}
// 	if (!player_found) {
// 		cleanup_and_exit(map, rows, "Error: No player spawn (N, E, S, W) found in the map.");
// 	}
// 	return map;
// }
