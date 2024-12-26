/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:00:53 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/26 17:31:16 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	traverse_and_execute(t_cmd_table *node, t_mini *attributes)
{
	static int	i;
	char	*cmd_path;
	int	pipe_fd[2];
	int	pid;

	if (!node)
		return ;
	if (node->type == t_pipe && node->left->type == t_pipe)
		traverse_and_execute(node->left, attributes);
	if (node->type == t_pipe && node->left->type == t_command)
	{
		i++;
		if (pipe(pipe_fd) == -1)
		{
			perror("piping error");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("forking error");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			dup2(pipe_fd[1], STDOUT_FILENO);
			close(pipe_fd[0]);
			cmd_path = get_command_path(node->left->cmd_arr[0], attributes);
			if (cmd_path)
			{
				if (execve(cmd_path, node->left->cmd_arr, attributes->envp_arr) == -1)
				{
					perror("execve error");
					//free mallocs
					exit(EXIT_FAILURE);
				}
			}
		}
		else
		{
			close(pipe_fd[1]);
			attributes->input_fd = pipe_fd[0];
		}
	}
	if (node->type == t_pipe && node->right->type == t_command)
	{
		i++;
		if (pipe(pipe_fd) == -1)
		{
			perror("piping error");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("forking error");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			dup2(attributes->input_fd, STDIN_FILENO);
			close(attributes->input_fd);
			dup2(pipe_fd[1], STDOUT_FILENO);
			close(pipe_fd[1]);
			cmd_path = get_command_path(node->right->cmd_arr[0], attributes);
			if (cmd_path)
			{
				if (execve(cmd_path, node->right->cmd_arr, attributes->envp_arr) == -1)//change the null here
				{
					perror("execve error");
					//free mallocs
					exit(EXIT_FAILURE);
				}
			}
		}
		else
		{
			close(pipe_fd[1]);
			attributes->input_fd = pipe_fd[0];
			waitpid(pid, NULL, 0);
		}
	}
	if (node->right != NULL && node->right->type == t_command)// && i == attributes->cmd_index)// last command
	{
		i++;
		dup2(attributes->input_fd, STDIN_FILENO);
		close(attributes->input_fd);
		cmd_path = get_command_path(node->right->cmd_arr[0], attributes);
		if (cmd_path)
		{
			if (execve(cmd_path, node->right->cmd_arr, attributes->envp_arr) == -1)//change the null here
			{
				perror("execve error");
				//free mallocs
				exit(EXIT_FAILURE);
			}
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
		printf("entering single command execution\n");
		single_command(attributes->commands, attributes);
	}
	else
	{
		printf("entering multi command execution\n");
		traverse_and_execute(attributes->commands, attributes);
	}
}
