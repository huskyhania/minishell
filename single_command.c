/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 20:07:20 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/16 23:50:33 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_single(char **cmd_array, t_mini *attributes, t_cmd_table *node)
{
	char	*cmd_path;
	if (node->type != t_command && node->type != 5)
	{
		if (check_files(node, attributes))
		{
			ft_free_ast(attributes);
			envp_cleanup(attributes);
			free(attributes->pids);
			exit(EXIT_FAILURE);
		}
	}
	if (node->in1 || node->last_infile == 5)
	{
		if (redir_in(node, attributes))
			cleanup_child(attributes);
	}
	if (node->out1)
	{
		if (redir_out(node, attributes))
			cleanup_child(attributes);
	}
	cmd_path = get_command_path(cmd_array[0], attributes);
	if (cmd_path)
	{
		if (execve(cmd_path, cmd_array, NULL) == -1)
		{
			perror("execve error");
			ft_free_ast(attributes);
			envp_cleanup(attributes);
			free(attributes->pids);
			exit(attributes->exitcode);
		}
	}
	else
	{
		envp_cleanup(attributes);
		ft_free_ast(attributes);
		free(attributes->pids);
		exit(attributes->exitcode);
	}
}

static void	handle_single(char **cmd_array, t_mini *attributes, t_cmd_table *node)
{
	int	pid;
	int	status;
	pid = fork();
	if (pid < 0)
	{
		printf("fork call failed");
		return ; // might require closing fds, freeing memory
	}
	if (pid == 0)
		execute_single(cmd_array, attributes, node);
	else
	{
		//close(attributes->here_fd);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			//printf("child exited with status %d\n", WEXITSTATUS(status));
			attributes->exitcode = WEXITSTATUS(status);
		}
		if (access("here_doc", F_OK) == 0)
			unlink("here_doc");
	}
}

void	single_command(t_cmd_table *node, t_mini *attributes)
{
	int	builtin_flag;
	int	i;

	i = 0;
	if (node->type != t_command)
	{
		if (check_files(node, attributes))
		{
			attributes->exitcode = 1;
			return;
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