/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:51:14 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/23 19:12:32 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_single(char **cmd_array, t_mini *attributes, t_cmd_table *node)
{
	char	*cmd_path;
	if (node->infile)
	{
		printf("input redir from %stest", node->infile);
		char **input_arr = ft_split(node->infile, ' ');
		int input = open(input_arr[0], O_RDONLY);
		if (input < 0)
		{
			printf("infile open err\n");//exitcode?
			exit(EXIT_FAILURE);
		}
		dup2(input, STDIN_FILENO);
		close(input);
	}
	if (node->outfile)
	{
		printf("output redir from %stest", node->outfile);
		char **output_arr = ft_split(node->outfile, ' ');
		int output = open(output_arr[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (output < 0)
		{
			printf("outfile open err\n"); //exitcode??
			exit(EXIT_FAILURE);
		}
		dup2(output, STDOUT_FILENO);
		close(output);
	}
	cmd_path = get_command_path(cmd_array[0], attributes);
	if (cmd_path)
	{
		if (execve(cmd_path, cmd_array, NULL) == -1)
		{
			perror("execve error");
			//free mallocs;
			printf("fail from execve call");
			exit(EXIT_FAILURE);
		}
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
	printf("%s command string", node->cmd_arr[0]);
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
