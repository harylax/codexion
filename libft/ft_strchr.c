/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haryandr <haryandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 09:56:25 by haryandr          #+#    #+#             */
/*   Updated: 2026/02/05 09:59:08 by haryandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	size_t			i;
	unsigned char	uc;
	size_t			len;

	len = ft_strlen(s);
	uc = (unsigned char) c;
	i = 0;
	if (uc == '\0')
		return ((char *) &s[len]);
	while (i < len)
	{
		if ((unsigned char)s[i] == uc)
			return ((char *) &s[i]);
		i++;
	}
	return (NULL);
}
