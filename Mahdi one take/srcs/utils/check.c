#include "../includes/cub3d.h"

int	check_format(char *str, char *check)
{
	int		i;
	char	*compare;

	i = ft_strlen(str);
	if (i > 4)
	{
		compare = &str[i - ft_strlen(check)];
		if (!ft_strncmp(compare, check, ft_strlen(check)))
			return (1);
	}
	return (0);
}
