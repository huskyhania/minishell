/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:00:53 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/17 20:57:14 by llaakson         ###   ########.fr       */
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

void	close_pipes_in_parent(t_mini *attributes)
{
	if (attributes->i > 1)
		close(attributes->pipe_arr[attributes->i - 2][READ]);
	if (attributes->i < attributes->cmd_index)
		close(attributes->pipe_arr[attributes->i - 1][WRITE]);
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
			if (check_files(node, attributes))
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

void	update_exit_recursion(t_mini *attributes)
{
	if (g_signal == SIGINT)
		attributes->exitcode = 130;
	else
		attributes->exitcode = 1;
}

void	handle_command(t_cmd_table *node, t_mini *attributes)
{
	attributes->i++;
	if (node->type != t_command && !node->cmd_arr)
	{
		if (check_files(node, attributes) == 1)
		{
			update_exit_recursion(attributes);
			return ;
		}
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
		else
			attributes->pids[attributes->i - 1] = -1;
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
	attributes->pids = NULL;
	attributes->pids = malloc(sizeof(int) * (attributes->cmd_index + 1));
	if (!attributes->pids)
		return (syscall_fail(1, attributes, "malloc"));
	attributes->pids[attributes->cmd_index] = 0;
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
