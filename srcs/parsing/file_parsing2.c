/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_parsing2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 14:21:18 by emkalkan          #+#    #+#             */
/*   Updated: 2025/02/26 18:36:07 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

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

void	free_rgb(t_data *data)
{
	fprintf(stderr, "Error: Invalid RGB value in .cub file.\n");
	free(data->map);
	free_textures(data, 0);
	free_maps(data);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	get_next_line(-1);
	exit(1);
}

int	parse_color(char *str, int i, t_data *data)
{
	int		r;
	int		g;
	int		b;
	char	*orig_line;
	char	*next;

	orig_line = str - 2;
	(void)i;
	while (*str && isspace(*str))
		str++;
	if (!*str || !ft_isdigit(*str))
		return (free(orig_line), free_rgb(data), -1);
	r = ft_atoi(str);
	next = strchr(str, ',');
	if (!next || !ft_isdigit(*(next + 1)))
		return (free(orig_line), free_rgb(data), -1);
	g = ft_atoi(next + 1);
	next = strchr(next + 1, ',');
	if (!next || !ft_isdigit(*(next + 1)))
		return (free(orig_line), free_rgb(data), -1);
	b = ft_atoi(next + 1);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (free(orig_line), free_rgb(data), -1);
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
			"MiniLibX Error: Cannot load texture for %s at tesst path: %s\n",
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
	// if (data->no_path == NULL || data->so_path == NULL
	// 	|| data->we_path == NULL || data->ea_path == NULL)
	// {
	// 	printf("Error: Missing texture path.\n");
	// 	free_textures(data, 0);
	// 	free_maps(data);
	// 	free(data->map);
	// 	mlx_destroy_display(data->mlx);
	// 	free(data->mlx);
	// 	get_next_line(-1);
	// 	exit(1);
	// }
}
