/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 17:31:31 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/16 17:20:32 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_handle_sigint(int sig)
{
	(void)sig;
	//write(1, "AA", 2);
	//rl_replace_line("", 0);
	//write(1,"\n", 1);
	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ft_sigint(void)
{
	signal(SIGINT, ft_handle_sigint);
	//signal(SIGQUIT, SIG_IGN);
}

void	ft_heredoc_sighandler(int sig)
{
	if (sig == SIGINT)
	{
		//write(1, "BB", 2);
		g_signal = SIGINT;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		close(STDIN_FILENO);
	}
}

void	ft_heresignal(void)
{
	signal(SIGINT, ft_heredoc_sighandler);
	signal(SIGQUIT, SIG_IGN);
}
