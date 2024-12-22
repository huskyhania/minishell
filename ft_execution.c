/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:00:53 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/22 18:30:53 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_empty_or_space(const char *cmd)
{
	if (!cmd || cmd[0] == '\0')
		return (1);
	while (*cmd)
	{
		if (*cmd != ' ')
			return (0);
		cmd++;
	}
	return (1);
}

char	**check_if_valid_command(const char *cmd)
{
	char **cmd_arr; // to be in struct
	if (is_empty_or_space(cmd))
	{
		write(2, cmd, ft_strlen(cmd));
		write(2, ": command not found\n", 21);
		//exitcode 127, possibly error flag?
		return (NULL);
	}
	cmd_arr = ft_split(cmd, ' ');
	if (!cmd_arr)
	{
		perror("cmd split error");
		return (NULL); // make sure to close fds if necessary, free cmd-related memory
	}
	int i = 0;
	while (cmd_arr[i] != NULL)
	{
		printf("cmd_arr[%d]: %s\n", i, cmd_arr[i]);
		i++;
	}
	return (cmd_arr);
}

void	execute_simple_command(char **cmd_arr, t_mini *attributes, t_cmd_table *node)
{
	char *cmd_path;
	if (node->infile)
	{
		printf("Input redirection from %s\n", node->infile);
		char **input_arr = ft_split(node->infile, ' ');//temp fix for extra space
		int input = open(input_arr[0], O_RDONLY);//open check
		if (input < 0)
		{
			printf("infile open err\n");
			return ;
		}
		printf("%d input var\n", input);
		dup2(input, STDIN_FILENO);
		close(input);
	}
	else if (attributes->input_fd != STDIN_FILENO)
	{
		dup2(attributes->input_fd, STDIN_FILENO);
		close(attributes->input_fd);
	}
	if (node->outfile)
	{
		printf("Output redirection from %s\n", node->outfile);
		char **output_arr = ft_split(node->outfile, ' ');
		int output = open(output_arr[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (output < 0)
		{
			printf("outfile open err\n\n\n\n");
			return ;
		}
		printf("output var %d\n", output);
		dup2(output, STDOUT_FILENO);
		close(output);
	}
	else if (attributes->output_fd != STDOUT_FILENO)
	{
		dup2(attributes->output_fd, STDOUT_FILENO);
		close(attributes->output_fd);
	}
	cmd_path = get_command_path(cmd_arr[0], attributes);
	if (cmd_path)
	{
		if (execve(cmd_path, cmd_arr, NULL) == -1)
		{
			perror("execve error");
			//free mallocs
			exit(EXIT_FAILURE);
		}
	}
}

void	handle_simple_command(char **cmd_arr, t_mini *attributes, t_cmd_table *node)
{
	int	pid;
	int	status;
	pid = fork();
	if (pid < 0)
	{
		printf("fork call failed");
		exit(EXIT_FAILURE);
		//might require closing file descriptors, freeing memory
	}
	if (pid == 0)
		execute_simple_command(cmd_arr, attributes, node);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			printf("child exited with status %d\n", WEXITSTATUS(status));
	}
}

void	traverse_and_execute(t_cmd_table *node, t_mini *attributes)
{
	char **cmd_array;
	int	builtin_flag;
	int	pipe_fd[2];
	if (!node)
		return ;
	if (pipe(pipe_fd) == -1)
		perror("piping issue");//return to minishell prompt
	if (node->type == t_pipe && node->left)
	{
		attributes->output_fd = pipe_fd[1];
		traverse_and_execute(node->left, attributes);
		attributes->input_fd = pipe_fd[0];
		close(pipe_fd[1]);
		traverse_and_execute(node->right, attributes);
		close(pipe_fd[0]);
	}
	else
	{
		printf("node content %s\n", node->str);
		cmd_array = check_if_valid_command(node->str);
		if (cmd_array)
		{
			builtin_flag = is_builtin(cmd_array[0]);
			if (builtin_flag != 0)
				handle_builtin(cmd_array, builtin_flag, attributes);
			else
				handle_simple_command(cmd_array, attributes, node);
			int i = 0;
			while (cmd_array[i] != NULL)
			{
				free(cmd_array[i]);
				i++;
			}
			free(cmd_array);
		}
	}
}

void	ft_execution(t_mini *attributes)
{
	int	builtin_flag;
	if (!attributes || !attributes->commands)
	{
		printf("something went wrong\n");
		return ;
	}
	if (attributes->commands->type == t_command)
	{
		char **cmd_array = check_if_valid_command(attributes->commands->str);
		if (cmd_array)
		{
			builtin_flag = is_builtin(cmd_array[0]);
			if (builtin_flag != 0)
				handle_builtin(cmd_array, builtin_flag, attributes);
			else
				handle_simple_command(cmd_array, attributes, attributes->commands);
			free_array(cmd_array);
		}
	}
	traverse_and_execute(attributes->commands, attributes);
}
