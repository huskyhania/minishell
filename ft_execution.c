/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:00:53 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/27 16:59:27 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void    handle_command_or_pipe(t_cmd_table *node, t_mini *attributes);

void	execute_command(t_cmd_table *node, t_mini *attributes, int output_fd)
{
	char	*cmd_path;

	if (attributes->input_fd != STDIN_FILENO)
	{
		dup2(attributes->input_fd, STDIN_FILENO);
		close(attributes->input_fd);
	}
	if (node->infile)
	{
		int infile_fd = open(node->infile, O_RDONLY);
		if (infile_fd == -1)
		{
			printf("infile err\n");
			exit(EXIT_FAILURE);
		}
		dup2(infile_fd, STDIN_FILENO);
		close(infile_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
	if (node->outfile)
	{
		int outfile_fd = open(node->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (outfile_fd == -1)
		{
			perror("outfile err\n");
			exit(EXIT_FAILURE);
		}
		dup2(outfile_fd, STDOUT_FILENO);
	}
	cmd_path = get_command_path(node->cmd_arr[0], attributes);
	if (cmd_path)
	{
		printf("test for execve cmd arr %p\n", node->cmd_arr);
		printf("test for execve cmd arr 0 %s \n", node->cmd_arr[0]);
		if (execve(cmd_path, node->cmd_arr, attributes->envp_arr) == -1)
		{
			perror("execve error");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		perror("command not found");
		exit(EXIT_FAILURE);
	}
}

void	handle_pipe(t_cmd_table *node, t_mini *attributes)
{
	int	pipe_fd[2];
	int	pid;
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe error");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork error");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		close(pipe_fd[READ]);
		execute_command(node->left, attributes, pipe_fd[WRITE]);
	}
	else
	{
		close(pipe_fd[WRITE]);
		if (attributes->input_fd != STDIN_FILENO)
			close(attributes->input_fd);
		attributes->input_fd = pipe_fd[READ];
		if (node->right)
			handle_command_or_pipe(node->right, attributes);
		waitpid(pid, NULL, 0);
		close(pipe_fd[READ]);
	}
}


void	handle_command_or_pipe(t_cmd_table *node, t_mini *attributes)
{
	int	pid;
	if (!node)
		return ;
	if (node->type == t_pipe)
		handle_pipe(node, attributes);
	else if (node->type != t_pipe)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork err");
			return;
		}
		if (pid == 0)
			execute_command(node, attributes, STDOUT_FILENO);
		else
		{
			if (attributes->input_fd != STDIN_FILENO)
				close(attributes->input_fd);
			waitpid(pid, NULL, 0);
		}
	}
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
		handle_command_or_pipe(attributes->commands, attributes);
	}
}
