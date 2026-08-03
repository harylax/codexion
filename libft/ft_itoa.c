/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haryandr <haryandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 09:53:53 by haryandr          #+#    #+#             */
/*   Updated: 2026/02/05 09:59:08 by haryandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	intlen(long n)
{
	int		len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		len++;
	while (n)
	{
		len++;
		n /= 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*new;
	int		len_n;
	long	nl;
	int		i;

	nl = (long) n;
	len_n = intlen(nl);
	new = malloc(sizeof(char) * (len_n + 1));
	if (!new)
		return (NULL);
	new[len_n] = '\0';
	if (nl < 0)
	{
		new[0] = '-';
		nl *= -1;
	}
	if (nl == 0)
		new[0] = '0';
	i = 0;
	while (nl)
	{
		new[len_n - 1 - i++] = (nl % 10) + '0';
		nl /= 10;
	}
	return (new);
}
