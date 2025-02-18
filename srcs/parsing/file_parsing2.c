/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_parsing2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emkalkan <emkalkan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 14:21:18 by emkalkan          #+#    #+#             */
/*   Updated: 2025/02/18 16:51:04 by emkalkan         ###   ########.fr       */
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
