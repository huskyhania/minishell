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
	handle_pipes(attributes);
	if (node->type != t_command && (node->in1 || node->last_infile == 5))
	{
		if (redir_in(node, attributes))
		{
			cleanup_child(attributes);
			exit(EXIT_FAILURE);
		}
	}
	if (node->type != t_command && node->out1)
	{
		if (redir_out(node, attributes))
		{
			cleanup_child(attributes);
			exit(EXIT_FAILURE);
		}
	}
	char *cmd_path = get_command_path(node->cmd_arr[0], attributes);
	if (cmd_path)
	{
		if (execve(cmd_path, node->cmd_arr, attributes->envp_arr) == -1)
		{
			free(cmd_path);
			cleanup_child(attributes);
			exit(attributes->exitcode);
		}
	}
	else if (!cmd_path)
	{
		cleanup_child(attributes);
		exit(attributes->exitcode);
	}
}

void	fork_for_command(t_cmd_table *node, t_mini *attributes)
{
	int	pid;
	int	builtin_flag;

	pid = fork();
	if (pid < 0)
	{
		syscall_fail(1, attributes, "fork");
		exit(EXIT_FAILURE);//might require closing fds, freeing memory)
	}
	if (pid == 0)
	{
		ft_resetsignal();
		if (node->type != t_command)
		{
			if (check_files(node, attributes))
			{
				cleanup_child(attributes);
				exit(EXIT_FAILURE);	
			}
		}
		builtin_flag = is_builtin(node->cmd_arr[0]);
		if (builtin_flag != 0)
		{
			handle_pipes(attributes);
			handle_builtin(node, builtin_flag, attributes);
		}	
		else
			execute_command(node, attributes);
	}
	else
	{
		if (attributes->i > 1)
			close(attributes->pipe_arr[attributes->i - 2][READ]);
		if (attributes->i < attributes->cmd_index)
			close(attributes->pipe_arr[attributes->i - 1][WRITE]);
		attributes->pids[attributes->i - 1] = pid;
	}
}
void	wait_for_all_processes(t_mini *attributes)
{
	int	status;
	int	i;
	int	signal_no;

	i = 0;
	status = 0x7F;
	while (attributes->pids[i] != 0)
	{
		if (attributes->pids[i] < 0)
			i++;
		else
		{
			waitpid(attributes->pids[i], &status, 0);
			if (WIFSIGNALED(status))
			{
				signal_no = WTERMSIG(status);
				//printf("%d signal noo \n", signal_no);
				attributes->exitcode = signal_no + 128;
				//printf("child exited with signal\n");
				if (signal_no != 13)
					return ;
			}
			if (WIFEXITED(status))
			{
				//printf("child exited with status %d\n", WEXITSTATUS(status));
				if (i == attributes->cmd_index - 1)
					attributes->exitcode = WEXITSTATUS(status);
			}
			i++;
		}
	}
}

void	handle_command(t_cmd_table *node, t_mini *attributes)
{
	attributes->i++;
	if (node->type != t_command && !node->cmd_arr)
	{
		if (check_files(node, attributes) == 1)
		{
			if (g_signal == SIGINT)
				attributes->exitcode = 130;
			else
				attributes->exitcode = 1;
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
	if (!attributes || !attributes->commands)
		return (syscall_fail(1, attributes, "attributes struct or node creation"));
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
			return(syscall_fail(1, attributes, "pipe"));
		handle_command_or_pipe(attributes->commands, attributes);
		if (g_signal == SIGINT)
			g_signal = 0;
		else
			wait_for_all_processes(attributes);
		free_pipes(attributes);
	}
	ft_free_ast(attributes);
	free(attributes->pids);
	attributes->pids = NULL;
}
