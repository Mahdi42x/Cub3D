/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_parsing2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 14:21:18 by emkalkan          #+#    #+#             */
/*   Updated: 2025/02/21 17:55:03 by mawada           ###   ########.fr       */
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

int	parse_color(char	*str, int i, t_data *data)
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
		free(data->map);
		free(data->no_path);
		free(data->so_path);
		free(data->we_path);
		free(data->ea_path);
		mlx_destroy_image(data->mlx, data->textures[0].img);
		mlx_destroy_image(data->mlx, data->textures[1].img);
		mlx_destroy_image(data->mlx, data->textures[2].img);
		mlx_destroy_image(data->mlx, data->textures[3].img);
		free_maps(data);
		mlx_destroy_display(data->mlx);
		free(data->mlx);
		exit(1);
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
