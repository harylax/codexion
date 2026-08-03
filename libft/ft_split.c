/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haryandr <haryandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 09:56:17 by haryandr          #+#    #+#             */
/*   Updated: 2026/02/05 09:59:08 by haryandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *s, char c)
{
	size_t	count;
	int		i;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			count++;
		i++;
	}
	return (count);
}

static void	free_split(char **new, size_t i)
{
	while (i > 0)
	{
		free(new[i - 1]);
		i--;
	}
	free(new);
}

static int	make_split(char const *s, char **new, char c)
{
	size_t			i;
	size_t			j;
	size_t			len;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
		{
			len = 0;
			while (s[i + len] && s[i + len] != c)
				len++;
			new[j] = ft_substr(s, i, len);
			if (!new[j])
			{
				free_split(new, j);
				return (0);
			}
			j++;
		}
		i++;
	}
	new[j] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char			**new;

	if (!s)
		return (NULL);
	new = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!new)
		return (NULL);
	if (!make_split(s, new, c))
		return (NULL);
	return (new);
}
