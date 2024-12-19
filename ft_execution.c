/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:00:53 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/19 15:29:57 by hskrzypi         ###   ########.fr       */
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

void	execute_simple_command(char **cmd_arr, t_mini *attributes)
{
	char *cmd_path;
	//if input was redirected
	//dup2(input_place, STDIN_FILENO);
	//if output was redirected
	//dup2(output_place, STDOUT_FILENO);
	//close(input_place);
	//close(output_place);
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

void	handle_simple_command(char **cmd_arr, t_mini *attributes)
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
		execute_simple_command(cmd_arr, attributes);
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
	if (!node)
		return ; //is it necessary?
	traverse_and_execute(node->left, attributes);
	if (node->type == t_command)
	{
		cmd_array = check_if_valid_command(node->str);
		if (cmd_array)
		{
			builtin_flag = is_builtin(cmd_array[0]);
			if (builtin_flag != 0)
				handle_builtin(cmd_array, builtin_flag, attributes);
			else
				handle_simple_command(cmd_array, attributes);
			int i = 0;
			while (cmd_array[i] != NULL)
			{
				free(cmd_array[i]);
				i++;
			}
			free(cmd_array);
		}
	}
	traverse_and_execute(node->right, attributes);
}

void	ft_execution(t_mini *attributes)
{
	if (!attributes || !attributes->commands)
	{
		printf("something went wrong\n");
		return ;
	}
	traverse_and_execute(attributes->commands, attributes);
}
	/*{
	char	**cmd_array;
	int	builtin_flag;
	t_cmd_table	*helper;
	int	i;
	builtin_flag = 0;
	i = 0;
	helper = attributes->commands;
	while (helper->type == t_pipe) // type for pipe 1
		helper = helper->left;
	if (helper->type == t_command)
	{
		cmd_array = check_if_valid_command(helper->str);
		if (cmd_array)
		{
			builtin_flag = is_builtin(cmd_array[0]);
			if (builtin_flag != 0)
				handle_builtin(cmd_array, builtin_flag, attributes);
			//if << was found and there is something afterwards -otherwise parse error
			//{
			//	printf("call to heredoc function");
			//	here_doc_handler(cmd_array);
			//}
			else
				handle_simple_command(cmd_array, attributes);
			int i = 0;
			while (cmd_array[i] != NULL)
			{
				free(cmd_array[i]);
				i++;
			}
			free(cmd_array);
		}
	}
}*/
