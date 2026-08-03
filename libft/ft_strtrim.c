/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haryandr <haryandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 09:57:57 by haryandr          #+#    #+#             */
/*   Updated: 2026/02/05 09:59:08 by haryandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_if_set(char const *set, char c)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static char	*empty_str(void)
{
	char	*new;

	new = malloc(1);
	if (!new)
		return (NULL);
	new[0] = '\0';
	return (new);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t			len;
	unsigned int	i;
	char			*new;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	while (s1[i] && check_if_set(set, s1[i]))
		i++;
	len = ft_strlen(s1);
	if (i == len)
	{
		new = empty_str();
		return (new);
	}
	len--;
	while (i < len && check_if_set(set, s1[len]))
		len--;
	new = ft_substr(s1, i, len - i + 1);
	return (new);
}
