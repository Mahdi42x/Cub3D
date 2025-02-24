/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_command_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawada <mawada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 13:08:46 by mawada            #+#    #+#             */
/*   Updated: 2025/02/24 16:50:46 by mawada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_ptr(unsigned long long nbr)
{
	int		count;
	char	*symbols;

	symbols = "0123456789abcdef";
	if (nbr < 16)
		return (print_char(symbols[nbr]));
	else
	{
		count = print_ptr(nbr / 16);
		return (count + print_char(symbols[nbr % 16]));
	}
}

int	print_pointer(va_list ap)
{
	unsigned long long int	num;
	int						count;

	count = 0;
	num = va_arg(ap, unsigned long long int);
	if (num == 0)
		count += print_str("(nil)");
	else
	{
		count += print_str("0x");
		count += print_ptr(num);
	}
	return (count);
}
