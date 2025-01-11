/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:00:53 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/09 21:33:31 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(t_cmd_table *node, t_mini *attributes)
{
	handle_pipes(attributes);
	if (node->type != t_command)
	{
		if (check_redirs(node, attributes))
			exit(EXIT_FAILURE);	
	}
	char *cmd_path = get_command_path(node->cmd_arr[0], attributes);
	//fprintf(stderr, "about to execute command %s\n", node->cmd_arr[0]);
	///fprintf(stderr, "exitcode in after path check %d\n", attributes->exitcode);
	//fprintf(stderr, "%s was path found\n", cmd_path);
	if (cmd_path)
	{
		if (execve(cmd_path, node->cmd_arr, attributes->envp_arr) == -1)
		{
			free(cmd_path);
			free_pipes(attributes);
			ft_free_ast(attributes);
			envp_cleanup(attributes);
			exit(attributes->exitcode);//exitfailure?
		}
	}
	else if (!cmd_path)
	{
		envp_cleanup(attributes);
		free_pipes(attributes);
		ft_free_ast(attributes);
		exit(attributes->exitcode);
	}
}

void	fork_for_command(t_cmd_table *node, t_mini *attributes)
{
	int	pid;
	int	status;
	int	builtin_flag;
	pid = fork();
	if (pid < 0)
	{
		perror("fork error");
		exit(EXIT_FAILURE);//might require closing fds, freeing memory)
	}
	if (pid == 0)
	{
		handle_pipes(attributes);
		if (node->type != t_command)
		{
			if (check_redirs(node, attributes))
				exit(EXIT_FAILURE);	
		}
		builtin_flag = is_builtin(node->cmd_arr[0]);
		if (builtin_flag != 0)
			handle_builtin(node, builtin_flag, attributes);	
		else
			execute_command(node, attributes);
	}
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
			//printf("child exited with status %d\n", WEXITSTATUS(status));
			attributes->exitcode = WEXITSTATUS(status);//can be conditional and only happen for the last command
		}
	}
}
void	handle_command(t_cmd_table *node, t_mini *attributes)
{
	//check path
	//check command
	//int	builtin_flag;
	attributes->i++;
	if (node->here && node->here != NULL)
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
	if (node->cmd_arr)
	{
		if (!check_if_valid_command(node, attributes))
		{
			//fprintf(stderr, "forking for command no %d amd it is %s\n", attributes->i, node->cmd_arr[0]);	
			fork_for_command(node, attributes);
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
		//printf("executing command %s\n", node->cmd_arr[0]);
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
		//if (attributes->commands->cmd_arr && attributes->commands->cmd_arr[0])
		//	printf("%sc command str\n", attributes->commands->cmd_arr[0]);
		//printf("entering single command execution\n");
		single_command(attributes->commands, attributes);
	}
	else
	{
		//printf("entering new command execution\n");
		//printf("creating pipes array");
		//printf("no of commands is %d\n", attributes->cmd_index);
		if (create_pipes(attributes) == -1)
		{
			printf("pipe creation error\n");//clean up;
			return ;
		}
		//else 
		//	printf("pipe creation success\n");
		handle_command_or_pipe(attributes->commands, attributes);
		free_pipes(attributes);
	}
	ft_free_ast(attributes);
}
