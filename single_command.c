/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 20:07:20 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/04 19:11:50 by hskrzypi         ###   ########.fr       */
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
			exit(attributes->exitcode);
		}
	}
	else
	{
	//	perror("command not found- path");
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
		perror("child process");
		execute_single(cmd_array, attributes, node);
	}
	else
	{
		//close(attributes->here_fd);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			printf("child exited with status %d\n", WEXITSTATUS(status));
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
	//attributes->array = check_if_valid_command(node->cmd_arr[0]);
	if (node->cmd_arr && node->cmd_arr[0])	
		printf("%s command string\n", node->cmd_arr[0]);
	if (node->infile && node->infile[0])
		printf("%sinfile str", node->infile[0]);
	if (node->here && node->here[i] != NULL)
	{
		while (node->here[i] != NULL)
		{
			attributes->here_fd = here_doc_handler(node, attributes, node->here[i]);
			if (attributes->here_fd < 0)
			{
				perror("heredoc error");
				attributes->exitcode = 1;
				return ; 
			}
			i++;
		}
	}
	if (attributes->commands->cmd_arr)
	{
		builtin_flag = is_builtin(node->cmd_arr[0]);
		if (builtin_flag != 0)
			handle_builtin(node->cmd_arr, builtin_flag, attributes);
		else
			handle_single(node->cmd_arr, attributes, node);
		free_array(node->cmd_arr);
		node->cmd_arr = NULL;
	}
}
