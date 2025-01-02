/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:00:53 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/02 20:11:58 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_redirections(t_cmd_table *node, t_mini *attributes)
{
	int	input;
	int	output;
	
	attributes->input_fd = STDIN_FILENO;//reseting fds
	attributes->output_fd = STDOUT_FILENO;
	if (node->infile)
	{
		input = open(node->infile, O_RDONLY);
		if (input == -1)
		{
			perror(node->infile);
			return (1);
		}
		else
			attributes->input_fd = input;
	}
	if (node->outfile)
	{
		output = open(node->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (output == -1)
		{
			perror(node->outfile);
			return (1);
		}
		else
			attributes->output_fd = output;
	}
	return (0);
}

void	execute_command(t_cmd_table *node, t_mini *attributes)
{
	if (attributes->i == 1)
	{
		perror("test1");
		dup2(attributes->pipe_arr[attributes->i - 1][WRITE], STDOUT_FILENO);
		close(attributes->pipe_arr[attributes->i - 1][WRITE]);
	}
	else if (attributes->i > 1 && attributes->i < attributes->cmd_index)
	{
		perror("test2");
		dup2(attributes->pipe_arr[attributes->i - 2][READ], STDIN_FILENO);
		close(attributes->pipe_arr[attributes->i - 2][READ]);
		dup2(attributes->pipe_arr[attributes->i - 1][WRITE], STDOUT_FILENO);
		close(attributes->pipe_arr[attributes->i - 1][WRITE]);
	}
	else if (attributes->i == attributes->cmd_index)
	{
		perror("test3");
		dup2(attributes->pipe_arr[attributes->i - 2][READ], STDIN_FILENO);
		close(attributes->pipe_arr[attributes->i - 2][READ]);
	}
	if (check_redirections(node, attributes))
	{
		attributes->exitcode = 1;
		perror("redirection check is failed\n");
		exit (1);//temporary fix
	}
	if (attributes->input_fd != STDIN_FILENO)
	{
		dup2(attributes->input_fd, STDIN_FILENO);
		close(attributes->input_fd);
	}
	if (attributes->output_fd != STDOUT_FILENO)
	{
		dup2(attributes->output_fd, STDOUT_FILENO);
		close(attributes->output_fd);
	}
	char *cmd_path = get_command_path(node->cmd_arr[0], attributes);
	fprintf(stderr, "about to execute command %s\n", node->cmd_arr[0]);
	printf("exitcode in after path check %d\n", attributes->exitcode);
	printf("%s was path found\n", cmd_path);
	if (cmd_path)
	{
		if (execve(cmd_path, node->cmd_arr, attributes->envp_arr) == -1)
		{
			free(cmd_path);
			exit(attributes->exitcode);//exitfailure?
		}
	}
	else if (!cmd_path)
		exit(127); // exitfailure??
}

void	handle_command(t_cmd_table *node, t_mini *attributes)
{
	//check path
	//check command
	int	pid;
	int	status;
	attributes->i++;
	printf("handling command %d and its %s\n", attributes->i, node->cmd_arr[0]);
	pid = fork();
	if (pid < 0)
	{
		perror("fork error");
		exit(EXIT_FAILURE);//might require closing fds, freeing memory)
	}
	if (pid == 0)
		execute_command(node, attributes);
	else
	{
		if (attributes->i > 1)
			close(attributes->pipe_arr[attributes->i - 2][READ]);
		if (attributes->i < attributes->cmd_index)
			close(attributes->pipe_arr[attributes->i - 1][WRITE]);
		//close(attributes->pipe_arr[attributes->i - 1][WRITE]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			printf("child exited with status %d\n", WEXITSTATUS(status));
			attributes->exitcode = WEXITSTATUS(status);
		}
	}
}	

void	handle_command_or_pipe(t_cmd_table *node, t_mini *attributes)
{
	//int	builtin_flag;
	if (!node)
		return ; //is it necessary?
	handle_command_or_pipe(node->left, attributes);
	if (node->type != t_pipe)
	{
		printf("executing command %s\n", node->cmd_arr[0]);
		handle_command(node, attributes);
		//attributes->cmd_index--;
		//printf("commands left to execute: %d\n", attributes->cmd_index);
	}
	handle_command_or_pipe(node->right, attributes);
}	

void	ft_execution(t_mini *attributes)
{
	if (!attributes || !attributes->commands)
	{
		printf("something went wrong\n");
		return ;
	}
	if (attributes->commands->type != t_pipe)
	{
		printf("%sc command str\n", attributes->commands->cmd_arr[0]);
		printf("entering single command execution\n");
		single_command(attributes->commands, attributes);
	}
	else
	{
		printf("entering new command execution\n");
		printf("creating pipes array");
		if (create_pipes(attributes) == -1)
		{
			printf("pipe creation error\n");//clean up;
			return ;
		}
		handle_command_or_pipe(attributes->commands, attributes);
		free_pipes(attributes);
	}
}
