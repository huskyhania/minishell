/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 17:31:31 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/23 13:53:23 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_sigint(void)
{
	struct sigaction	sa;

	if (sigemptyset(&sa.sa_mask) == -1)
		ft_putstr_fd("Failed to set signal\n", 2);
	sa.sa_handler = ft_handle_sigint;
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		ft_putstr_fd("Failed to set signal\n", 2);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
}

void	ft_sigignore(void)
{
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
}

void	ft_resetsignal(void)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
}

void	ft_heresignal(void)
{
	if (signal(SIGINT, ft_heredoc_sighandler) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
}

void	ft_resetposthere(void)
{
	if (signal(SIGINT, ft_handle_post_here) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		ft_putstr_fd("Failed to set signal\n", 2);
}
