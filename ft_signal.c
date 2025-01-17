/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 17:31:31 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/17 18:13:05 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_set_exit(void)
{
	return (0);
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
		rl_replace_line("", 0);
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_redisplay();
		(ft_attributes()->exitcode) = 130;
	}
}

void	ft_sigint(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = ft_handle_sigint;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}
/*void	ft_sigint(void)
{
	//rl_done = 0;
	//rl_event_hook = NULL;
	signal(SIGINT, ft_handle_sigint);
	signal(SIGQUIT, SIG_DFL);
}*/

void	ft_heredoc_sighandler(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = SIGINT;
		//ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		//rl_done = 1;
		(ft_attributes()->exitcode) = 130;
		close(STDIN_FILENO);
	}
}

void	ft_resetsignal(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
void	ft_heresignal(void)
{
	//rl_event_hook = ft_event;
	signal(SIGINT, ft_heredoc_sighandler);
	signal(SIGQUIT, SIG_IGN);
}

void ft_resetposthere(void)
{
	signal(SIGINT,ft_handle_post_here);
	signal(SIGQUIT, SIG_DFL);
}
