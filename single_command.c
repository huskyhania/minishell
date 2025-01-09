/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 20:07:20 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/09 21:04:51 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_single(char **cmd_array, t_mini *attributes, t_cmd_table *node)
{
	char	*cmd_path;
	if (node->type != t_command)
	{
		if (check_redirs(node, attributes))
			exit(EXIT_FAILURE);
	}
	cmd_path = get_command_path(cmd_array[0], attributes);
	if (cmd_path)
	{
		if (execve(cmd_path, cmd_array, NULL) == -1)
		{
			perror("execve error");
			envp_cleanup(attributes);
			exit(attributes->exitcode);
		}
	}
	else
	{
	//	perror("command not found- path");
		envp_cleanup(attributes);
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
	{
		//perror("child process");
		execute_single(cmd_array, attributes, node);
	}
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
	envp_cleanup(attributes);
}

void	single_command(t_cmd_table *node, t_mini *attributes)
{
	int	builtin_flag;
	int	i;

	i = 0;
	if (node->here && node->here[i] != NULL)
	{
		if (process_heredocs(node, attributes))
		{
			attributes->exitcode = 1;
			return ;
		}
	}
	if (!node->cmd_arr && node->type != t_command)
	{
		redir_empty(node, attributes);
		unlink("here_doc");
	}
	else if (attributes->commands->cmd_arr)
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
