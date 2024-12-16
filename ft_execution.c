/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:00:53 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/16 15:20:28 by hskrzypi         ###   ########.fr       */
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

void	ft_execution(t_mini *attributes)
{
	char	**cmd_array;
	int	builtin_flag;
	builtin_flag = 0;
	//attributes->envp_heap = NULL;
	//attributes->envp_heap = envp_to_list(envp);
	//attributes->envp_arr = envp_to_array(attributes.envp_heap);
	//cmd_array = attributes->commands;
	if (1)
	{
		cmd_array = check_if_valid_command(attributes->commands->str);
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
}
/*
int main(int argc, char **argv, char **envp)
{
	char	**cmd_arr;
	cmd_arr = NULL;
	if (argc == 2)
		cmd_arr = check_if_valid_command(argv[1]);
	if (cmd_arr)
	{
		handle_simple_command(cmd_arr, envp);
		int i = 0;
		while (cmd_arr[i] != NULL)
		{
			free(cmd_arr[i]);
			i++;
		}
		free(cmd_arr);
	}
	return (0);
}*/
