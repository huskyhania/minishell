/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 20:07:20 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/09 21:58:09 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_single(char **cmd_array, t_mini *attributes, t_cmd_table *node)
{
	char	*cmd_path;
	(void)node;
	if (node->type != t_command)
	{
		if (check_files(node, attributes))
		{
			ft_free_ast(attributes);
			envp_cleanup(attributes);
			exit(EXIT_FAILURE);
		}
	}
	if (attributes->input_fd > 0)
	{	
		dup2(attributes->input_fd, STDIN_FILENO);
		close(attributes->input_fd);
	}
	if (attributes->output_fd > 1)
	{
		dup2(attributes->output_fd, STDOUT_FILENO);
		close(attributes->output_fd);
	}
	cmd_path = get_command_path(cmd_array[0], attributes);
	if (cmd_path)
	{
		if (execve(cmd_path, cmd_array, NULL) == -1)
		{
			perror("execve error");
			ft_free_ast(attributes);
			envp_cleanup(attributes);
			exit(attributes->exitcode);
		}
	}
	else
	{
		envp_cleanup(attributes);
		ft_free_ast(attributes);
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
		//printf("comand string is %s", node->cmd_arr[0]);
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