/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 11:55:19 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/23 13:44:41 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_signal_error(int sig)
{
	if (sig == 130)
		ft_putstr_fd("\n", 2);
	if (sig == 131)
		ft_putstr_fd("Quit (core dumped)\n", 2);
	else if (sig == 139)
		ft_putstr_fd("Segmentation fault (core dumped)\n", 2);
}

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
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		(ft_attributes()->exitcode) = 130;
	}
}

void	ft_heredoc_sighandler(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = SIGINT;
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		(ft_attributes()->exitcode) = 130;
		close(STDIN_FILENO);
	}
}
