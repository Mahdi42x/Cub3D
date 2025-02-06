/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:35:41 by mawada            #+#    #+#             */
/*   Updated: 2025/02/06 13:45:11 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	handle_texture(char *line, t_data *data)
{
	if (strncmp(line, "NO ", 3) == 0)
	{
		data->no_path = ft_strtrim(line + 3, " \n\t");
		load_texture(data, &data->textures[3], line + 3);
	}
	else if (strncmp(line, "SO ", 3) == 0)
	{
		data->so_path = ft_strtrim(line + 3, " \n\t");
		load_texture(data, &data->textures[2], line + 3);
	}
	else if (strncmp(line, "WE ", 3) == 0)
	{
		data->we_path = ft_strtrim(line + 3, " \n\t");
		load_texture(data, &data->textures[1], line + 3);
	}
	else if (strncmp(line, "EA ", 3) == 0)
	{
		data->ea_path = ft_strtrim(line + 3, " \n\t");
		load_texture(data, &data->textures[0], line + 3);
	}
}

void	handle_color(char *line, t_data *data)
{
	if (strncmp(line, "F ", 2) == 0)
		data->floor_color = parse_color(line + 2, 0);
	else if (strncmp(line, "C ", 2) == 0)
		data->ceiling_color = parse_color(line + 2, 1);
}

void	parse_maps(t_data *data, char *line, int fd)
{
	data->map = parse_map_from_line(line, fd, data);
	free(line);
}

void	set_map_width(t_data *data)
{
	int	i;
	int	len;

	data->map_width = 0;
	i = 0;
	while (data->map[i])
	{
		len = strlen(data->map[i]);
		if (len > data->map_width)
			data->map_width = len;
		i++;
	}
}

void	parse_cub_file(t_data *data, const char *file_path)
{
	int		fd;
	char	*line;

	fd = open(file_path, O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	line = get_next_line(fd);
	while (line)
	{
		if (strncmp(line, "NO ", 3) == 0 || strncmp(line, "SO ", 3) == 0
			|| strncmp(line, "WE ", 3) == 0 || strncmp(line, "EA ", 3) == 0)
			handle_texture(line, data);
		else if (strncmp(line, "F ", 2) == 0 || strncmp(line, "C ", 2) == 0)
			handle_color(line, data);
		else if (*line == '1' || *line == '0' || *line == ' ')
		{
			parse_maps(data, line, fd);
			break ;
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	if (!data->map || !data->map[0])
	{
		fprintf(stderr, "Error: Map data is missing or empty in the .cub file.\n");
		exit(EXIT_FAILURE);
	}
	set_map_width(data);
}

int	is_cub_file(char *file_path)
{
	size_t	length;

	length = strlen(file_path);
	if (length >= 4)
	{
		if (strncmp(file_path + length - 4, ".cub", 4) == 0)
			return (1);
	}
	printf("The map does not have the correct ending '.cub'\n");
	return (0);
}

int	parse_color(char	*str, int i)
{
	int	r;
	int	g;
	int	b;

	(void) i;
	while (*str && isspace(*str))
		str++;
	r = ft_atoi(str);
	str = strchr(str, ',') + 1;
	g = ft_atoi(str);
	str = strchr(str, ',') + 1;
	b = ft_atoi(str);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
	{
		fprintf(stderr, "Error: Invalid RGB value in .cub file.\n");
		exit(EXIT_FAILURE);
	}
	return ((r << 16) | (g << 8) | b);
}

void	test_texture_loading(void	*mlx, char	*path, const char	*label)
{
	int		width;
	int		height;
	void	*img;

	img = mlx_xpm_file_to_image(mlx, path, &width, &height);
	if (!img)
	{
		perror("System Error");
		fprintf(stderr,
			"MiniLibX Error: Cannot load texture for %s at path: %s\n",
			label, path);
		exit(EXIT_FAILURE);
	}
	else
		mlx_destroy_image(mlx, img);
}

void	test_all_textures(t_data *data)
{
	if (data->no_path)
		test_texture_loading(data->mlx, data->no_path, "NO");
	if (data->so_path)
		test_texture_loading(data->mlx, data->so_path, "SO");
	if (data->we_path)
		test_texture_loading(data->mlx, data->we_path, "WE");
	if (data->ea_path)
		test_texture_loading(data->mlx, data->ea_path, "EA");
}
