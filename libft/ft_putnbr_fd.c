/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haryandr <haryandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 09:56:02 by haryandr          #+#    #+#             */
/*   Updated: 2026/02/05 09:59:08 by haryandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long	nl;

	if (fd < 0)
		return ;
	nl = (long) n;
	if (nl < 0)
	{
		ft_putchar_fd('-', fd);
		nl *= -1;
	}
	if (nl > 9)
		ft_putnbr_fd(nl / 10, fd);
	ft_putchar_fd((nl % 10) + '0', fd);
}
