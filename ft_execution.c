/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:00:53 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/23 13:40:07 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(t_cmd_table *node, t_mini *attributes)
{
	char	*cmd_path;

	if (node->type != t_command)
		redirect_child(node, attributes);
	cmd_path = get_command_path(node->cmd_arr[0], attributes);
	if (cmd_path)
	{
		if (execve(cmd_path, node->cmd_arr, attributes->envp_arr) == -1)
			free(cmd_path);
	}
	cleanup_child(attributes);
	exit(attributes->exitcode);
}

void	fork_for_command(t_cmd_table *node, t_mini *attributes)
{
	int	builtin_flag;

	attributes->pids[attributes->i - 1] = fork();
	if (attributes->pids[attributes->i - 1] < 0)
		return (syscall_fail(1, attributes, "fork"));
	if (attributes->pids[attributes->i - 1] == 0)
	{
		ft_resetsignal();
		handle_pipes(attributes);
		if (node->type != t_command)
		{
			if (check_files(node, attributes, -1))
			{
				cleanup_child(attributes);
				exit (EXIT_FAILURE);
			}
		}
		builtin_flag = is_builtin(node->cmd_arr[0]);
		if (builtin_flag != 0)
			handle_builtin(node, builtin_flag, attributes);
		else
			execute_command(node, attributes);
	}
	else
		close_pipes_in_parent(attributes);
}

void	handle_command(t_cmd_table *node, t_mini *attributes)
{
	attributes->i++;
	if (node->type != t_command && !node->cmd_arr)
	{
		if (check_files(node, attributes, -1) == 1)
		{
			update_exit_recursion(attributes);
			attributes->pids[attributes->i - 1] = -1;
			close_pipes_in_parent(attributes);
			return ;
		}
		close_pipes_in_parent(attributes);
	}
	if (node->cmd_arr)
	{
		if (!check_if_valid_command(node, attributes))
		{
			if (node->type != t_command)
			{
				check_for_heredocs(node, attributes);
				if (g_signal == SIGINT)
					return ;
			}
			fork_for_command(node, attributes);
		}
	}
}

void	handle_command_or_pipe(t_cmd_table *node, t_mini *attributes)
{
	if (!node)
		return ;
	if (g_signal == SIGINT)
		return ;
	handle_command_or_pipe(node->left, attributes);
	if (node->type != t_pipe)
		handle_command(node, attributes);
	handle_command_or_pipe(node->right, attributes);
}

void	ft_execution(t_mini *attributes)
{
	ft_sigint();
	if (create_pids_array(attributes))
		return (syscall_fail(1, attributes, "malloc"));
	if (attributes->commands->type != t_pipe)
	{
		single_command(attributes->commands, attributes);
		if (g_signal == SIGINT)
			g_signal = 0;
	}
	else
	{
		if (create_pipes(attributes) == -1)
			return (syscall_fail(1, attributes, "pipe"));
		handle_command_or_pipe(attributes->commands, attributes);
		if (g_signal == SIGINT)
			g_signal = 0;
		else
			wait_for_all_processes(attributes);
		free_pipes(attributes);
	}
	cleanup_parent(attributes);
}
