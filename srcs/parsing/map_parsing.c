/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:18:31 by mawada            #+#    #+#             */
/*   Updated: 2025/02/26 19:04:42 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	add_line_to_map(char ***map, char *line, int *rows)
{
	char	**new_map;
	size_t	len;

	len = strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	new_map = realloc(*map, sizeof(char *) * (*rows + 2));
	if (new_map == NULL)
	{
		perror("Error reallocating map");
		exit(EXIT_FAILURE);
	}
	*map = new_map;
	(*map)[*rows] = strdup(line);
	if ((*map)[*rows] == NULL)
	{
		perror("Error duplicating line");
		exit(EXIT_FAILURE);
	}
	(*rows)++;
	(*map)[*rows] = NULL;
}

char	**parse_map(int fd)
{
	char	**map;
	char	*line;
	int		rows;

	map = NULL;
	rows = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (rows == 0 && strspn(line, " \n") == strlen(line))
		{
			free(line);
			continue ;
		}
		add_line_to_map(&map, line, &rows);
		free(line);
	}
	return (map);
}

void	parse_cub_file_loop(t_data *data, char *line, int fd)
{
	int has_fc;

	has_fc = 0;
	while (line)
	{
		if (strncmp(line, "NO ", 3) == 0 || strncmp(line, "SO ", 3) == 0
			|| strncmp(line, "WE ", 3) == 0 || strncmp(line, "EA ", 3) == 0)
			handle_texture(line, data);
		else if (strncmp(line, "F ", 2) == 0 || strncmp(line, "C ", 2) == 0)
		{
			handle_color(line, data);
			has_fc = 1;
		}
		else if (*line == '1' || *line == '0' || *line == ' ')
		{
			if (!has_fc)
			{
				printf("Error: Missing 'F' or 'C' in configuration.\n");
				free(line);
				close(fd);
				exit(1);
			}
			parse_maps(data, line, fd);
			if (!is_map_enclosed(data->map, data))
			{
				printf("Error: The map is not enclosed by walls.\n");
				break ;
			}
			break ;
		}
		free(line);
		line = get_next_line(fd);
	}
}

void	parse_cub_file(t_data *data, const char *file_path)
{
	int		fd;
	char	*line;

	fd = open(file_path, O_RDONLY);
	if (fd == -1)
	{
		printf("Error opening file");
		exit (1);
	}
	line = get_next_line(fd);
	if (*line == 32 || *line == 0)
	{
		printf("Error reading file");
		free(line);
		mlx_destroy_display(data->mlx);
		free(data->mlx);
		free(data->map);
		close(fd);
		exit(1);
	}
	parse_cub_file_loop(data, line, fd);
	close(fd);
	parse_cub_file_helper(data);
}
