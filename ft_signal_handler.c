/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 11:55:19 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/20 12:00:14 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_handle_post_here(int sig)
{
	if (sig == SIGINT)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		(ft_attributes()->exitcode) = 130;
	}
}

void	ft_handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_redisplay();
		(ft_attributes()->exitcode) = 130;
	}
}

void	ft_heredoc_sighandler(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = SIGINT;
		rl_on_new_line();
		rl_replace_line("", 0);
		(ft_attributes()->exitcode) = 130;
		close(STDIN_FILENO);
	}
}
