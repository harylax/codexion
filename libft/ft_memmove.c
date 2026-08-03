/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haryandr <haryandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 09:55:31 by haryandr          #+#    #+#             */
/*   Updated: 2026/02/05 09:59:08 by haryandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*ptr_d;
	unsigned char	*ptr_s;

	ptr_d = (unsigned char *) dest;
	ptr_s = (unsigned char *) src;
	if (ptr_d > ptr_s)
	{
		while (n > 0)
		{
			ptr_d[n - 1] = ptr_s[n - 1];
			n--;
		}
	}
	else
		return (ft_memcpy(dest, src, n));
	return (dest);
}
