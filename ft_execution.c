/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:00:53 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/15 20:41:22 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(t_cmd_table *node, t_mini *attributes)
{
	handle_pipes(attributes);
	if (node->type != t_command && (node->in1 || node->last_infile == 5))
	{
		if (node->last_infile == 3)
			node->input_fd = open(node->in1, O_RDONLY);
		if (node->last_infile == 5)
			node->input_fd = open("here_doc", O_RDONLY);
		if (dup2(node->input_fd, STDIN_FILENO) == -1)
			perror("dup2 fail for input");
		close(node->input_fd);
	}
	if (node->type != t_command && node->out1)
	{
		if (node->last_outfile == 2)
			node->output_fd = open(node->out1, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (node->last_outfile == 4)
			node->output_fd = open(node->out1, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (dup2(node->output_fd, STDOUT_FILENO) == -1)
			perror("dup2 fail for output");
		close(node->output_fd);
	}
	char *cmd_path = get_command_path(node->cmd_arr[0], attributes);
	if (cmd_path)
	{
		if (execve(cmd_path, node->cmd_arr, attributes->envp_arr) == -1)
		{
			free(cmd_path);
			free_pipes(attributes);
			ft_free_ast(attributes);
			envp_cleanup(attributes);
			free(attributes->pids);
			exit(attributes->exitcode);
		}
	}
	else if (!cmd_path)
	{
		envp_cleanup(attributes);
		free_pipes(attributes);
		ft_free_ast(attributes);
		free(attributes->pids);
		exit(attributes->exitcode);
	}
}
void	fork_for_command(t_cmd_table *node, t_mini *attributes)
{
	int	pid;
	//int	status;
	int	builtin_flag;
	pid = fork();
	if (pid < 0)
	{
		perror("fork error");
		exit(EXIT_FAILURE);//might require closing fds, freeing memory)
	}
	if (pid == 0)
	{
		if (node->type != t_command)
		{
			if (check_files(node, attributes))
			{
				envp_cleanup(attributes);
				ft_free_ast(attributes);
				free_pipes(attributes);
				free(attributes->pids);
				exit(EXIT_FAILURE);	
			}
		}
		builtin_flag = is_builtin(node->cmd_arr[0]);
		if (builtin_flag != 0)
		{
			handle_pipes(attributes);
			handle_builtin(node, builtin_flag, attributes);
		}	
		else
			execute_command(node, attributes);
	}
	else
	{
		if (attributes->i > 1)
			close(attributes->pipe_arr[attributes->i - 2][READ]);
		if (attributes->i < attributes->cmd_index)
			close(attributes->pipe_arr[attributes->i - 1][WRITE]);
		attributes->pids[attributes->i - 1] = pid;
	}
}
void	wait_for_all_processes(t_mini *attributes)
{
	int	status;
	int	i;

	i = 0;
	while (attributes->pids[i] != 0)
	{
		if (attributes->pids[i] < 0)
			i++;
		else
		{
			waitpid(attributes->pids[i], &status, 0);
			if (WIFEXITED(status))
			{
				printf("child exited with status %d\n", WEXITSTATUS(status));
				if (i == attributes->cmd_index - 1)
					attributes->exitcode = WEXITSTATUS(status);
			}
			i++;
		}
	}
}

void	handle_command(t_cmd_table *node, t_mini *attributes)
{
	attributes->i++;
	node->input_fd = 0;
	node->output_fd = 1;
	node->in1 = NULL;
	node->out1 = NULL;
	if (node->type != t_command && !node->cmd_arr)
	{
		if (check_files(node, attributes) == 1)
		{
			attributes->exitcode = 1;
			return ;
		}
	}
	if (node->cmd_arr)
	{
		if (!check_if_valid_command(node, attributes))
		{
			if (node->type != t_command)
			{
				check_for_heredocs(node, attributes);
				if (g_signal == SIGINT)
					return ;
			}
			fork_for_command(node, attributes);
		}
		else
			attributes->pids[attributes->i - 1] = -1;
	}
}

void	handle_command_or_pipe(t_cmd_table *node, t_mini *attributes)
{
	if (!node)
		return ;
	if (g_signal == SIGINT)
	{
		//g_signal = 0;
		return ;
	}
	handle_command_or_pipe(node->left, attributes);
	if (node->type != t_pipe)
	{
		handle_command(node, attributes);
	}
	handle_command_or_pipe(node->right, attributes);
}

void	ft_execution(t_mini *attributes)
{
	attributes->pids = NULL;
	if (!attributes || !attributes->commands)
	{
		printf("something went wrong\n");
		return ;
	}
	attributes->pids = malloc(sizeof(int) * (attributes->cmd_index + 1));
	attributes->pids[attributes->cmd_index] = 0;
	if (!attributes->pids)
	{
		ft_putstr_fd("malloc error\n", 2);
		return ;
	}
	if (attributes->commands->type != t_pipe)
		single_command(attributes->commands, attributes);
	else
	{
		if (create_pipes(attributes) == -1)
		{
			printf("pipe creation error\n");
			return ;
		}
		handle_command_or_pipe(attributes->commands, attributes);
		if (g_signal == SIGINT)
			g_signal = 0;
		else
			wait_for_all_processes(attributes);
		free_pipes(attributes);
	}
	ft_sigint();
	ft_free_ast(attributes);
	free(attributes->pids);
}
