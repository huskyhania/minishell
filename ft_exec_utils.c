/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 12:18:47 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/22 12:21:39 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_pids_array(t_mini *attributes)
{
	int	i;

	i = 0;
	attributes->pids = NULL;
	attributes->pids = malloc(sizeof(int) * (attributes->cmd_index + 1));
	if (!attributes->pids)
		return (1);
	while (i < attributes->cmd_index)
	{
		attributes->pids[i] = -1;
		i++;
	}
	attributes->pids[attributes->cmd_index] = 0;
	return (0);
}

void	update_exit_recursion(t_mini *attributes)
{
	if (g_signal == SIGINT)
		attributes->exitcode = 130;
	else
		attributes->exitcode = 1;
}

void	close_pipes_in_parent(t_mini *attributes)
{
	if (attributes->i > 1)
		close(attributes->pipe_arr[attributes->i - 2][READ]);
	if (attributes->i < attributes->cmd_index)
		close(attributes->pipe_arr[attributes->i - 1][WRITE]);
}

void	wait_for_all_processes(t_mini *attributes)
{
	int	status;
	int	i;
	int	signal_no;

	i = 0;
	status = 0x7F;
	if (!attributes->pids)
		return ;
	while (attributes->pids[i] != 0)
	{
		if (attributes->pids[i] > 0)
		{
			waitpid(attributes->pids[i], &status, 0);
			if (WIFSIGNALED(status))
			{
				signal_no = WTERMSIG(status);
				attributes->exitcode = signal_no + 128;
				if (signal_no != 13)
					return ;
			}
			if (WIFEXITED(status))
				update_exitcode(WEXITSTATUS(status), attributes);
		}
		i++;
	}
}
