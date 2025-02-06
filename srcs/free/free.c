/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:04:22 by mawada            #+#    #+#             */
/*   Updated: 2025/02/06 17:56:54 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

void	free_and_exit(void *ptr, const char *error_msg, int exit_code)
{
	if (ptr)
		free(ptr);
	if (error_msg)
		fprintf(stderr, "Error: %s\n", error_msg);
	exit(exit_code);
}

void	freethis(void *ptr)
{
	
}
