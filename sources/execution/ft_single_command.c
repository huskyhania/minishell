/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 20:07:20 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/23 13:42:13 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_child(t_cmd_table *node, t_mini *attributes)
{
	if (node->type != t_command && node->type != 5
		&& attributes->cmd_index == 1)
	{
		if (check_files(node, attributes, -1))
		{
			cleanup_child(attributes);
			exit(EXIT_FAILURE);
		}
	}
	if (node->in1 || node->last_infile == 5)
	{
		if (redir_in(node, attributes))
		{
			cleanup_child(attributes);
			exit(EXIT_FAILURE);
		}
	}
	if (node->out1)
	{
		if (redir_out(node, attributes))
		{
			cleanup_child(attributes);
			exit(EXIT_FAILURE);
		}
	}
}

void	execute_single(char **cmd_arr, t_mini *attributes, t_cmd_table *node)
{
	char	*cmd_path;

	if (node->type != t_command)
		redirect_child(node, attributes);
	cmd_path = get_command_path(cmd_arr[0], attributes);
	if (cmd_path)
	{
		if (execve(cmd_path, cmd_arr, attributes->envp_arr) == -1)
		{
			free(cmd_path);
			cmd_path = NULL;
		}
	}
	cleanup_child(attributes);
	exit(attributes->exitcode);
}

static void	handle_single(char **cmd_array, t_mini *attribs, t_cmd_table *node)
{
	ft_sigignore();
	attribs->pids[0] = fork();
	if (attribs->pids[0] < 0)
		return (syscall_fail(1, attribs, "fork"));
	if (attribs->pids[0] == 0)
	{
		ft_resetsignal();
		execute_single(cmd_array, attribs, node);
	}
	else
		wait_for_all_processes(attribs);
	ft_sigint();
}

void	single_command(t_cmd_table *node, t_mini *attributes)
{
	int	builtin_flag;

	if (node->type != t_command)
	{
		if (check_files(node, attributes, -1))
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
		builtin_flag = is_builtin(node->cmd_arr[0]);
		if (builtin_flag != 0)
			handle_builtin(node, builtin_flag, attributes);
		else
		{
			if (!check_if_valid_command(node, attributes))
				handle_single(node->cmd_arr, attributes, node);
		}
	}
}
