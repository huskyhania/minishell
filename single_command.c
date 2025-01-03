/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 20:07:20 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/03 20:38:22 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_redirs(t_cmd_table *node, t_mini *attributes)
{
	int	input;
	int	output;
	int	here_doc_fd;
	if (node->infile && node->infile[0])
	{
		input = open(node->infile[0], O_RDONLY);
		if (input < 0)
		{
			perror(node->infile[0]);
			attributes->exitcode = 1;
			return (1);
		}
		else
		{
			dup2(input, STDIN_FILENO);
			close(input);
		}
	}
	if (node->here && node->here[0])
	{
		here_doc_fd = here_doc_handler(node, attributes);
		if (here_doc_fd < 0)
		{
			perror("here doc function fail");
			exit (1);
		}
		printf("%d here doc fd\n", here_doc_fd);
		if (dup2(here_doc_fd, STDIN_FILENO) < 0)
			perror("dup2 error");
		close(here_doc_fd);
	}
	if (node->outfile && node->outfile[0])
	{
		output = open(node->outfile[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (output < 0)
		{
			perror(node->outfile[0]);
			attributes->exitcode = 1;
			return (1);
		}
		else
		{
			dup2(output, STDOUT_FILENO);
			close(output);
		}
	}
	return (0);
}

static void	execute_single(char **cmd_array, t_mini *attributes, t_cmd_table *node)
{
	char	*cmd_path;
	if (check_redirs(node, attributes))
		exit(EXIT_FAILURE);
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
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			printf("child exited with status %d\n", WEXITSTATUS(status));
			attributes->exitcode = WEXITSTATUS(status);
		}
	}
}

void	single_command(t_cmd_table *node, t_mini *attributes)
{
	int	builtin_flag;

	//attributes->array = check_if_valid_command(node->cmd_arr[0]);
	if (node->cmd_arr && node->cmd_arr[0])	
		printf("%s command string\n", node->cmd_arr[0]);
	if (node->infile && node->infile[0])
		printf("%sinfile str", node->infile[0]);
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
