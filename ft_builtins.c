/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:00:04 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/10 14:18:57 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(t_mini *attributes)
{
	char	*cwd;

	cwd = NULL;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_putstr_fd("pwd: error retrieving current directory", 2);
		attributes->exitcode = 1;
		return ;
	}
	printf("%s\n", cwd);
	free(cwd);
	attributes->exitcode = 0;
}

void	ft_env(t_cmd_table *node, t_mini *attributes)
{
	char	*path_value;
	int	saved_std;

	path_value = get_env_value(attributes, "PATH");
	saved_std = dup(STDOUT_FILENO);
	if (attributes->cmd_index > 1)
	{
		if (attributes->i < attributes->cmd_index)
		{
			dup2(attributes->pipe_arr[attributes->i - 1][WRITE], STDOUT_FILENO);
			close(attributes->pipe_arr[attributes->i - 1][WRITE]);
		}
		if (attributes->i > 1)
			close(attributes->pipe_arr[attributes->i - 2][READ]);
	}	
	if (node->outfile)
	{
		if (check_outfile(node, attributes))
		{
			attributes->exitcode = 1;
			return ;
		}
	}
	if (node->append)
	{
		if (check_append(node, attributes))
		{
			attributes->exitcode = 1;
			return ;
		}
	}

	if (!path_value || path_value[0] == '\0')
	{
		ft_putstr_fd("env: No such file or directory", 2);
		attributes->exitcode = 127;
		return ;
	}
	else
	{
		print_envp_list(attributes->envp_heap);
		attributes->exitcode = 0;
	}
	dup2(saved_std, STDOUT_FILENO);
	close(saved_std);
}

void	ft_echo(t_cmd_table *node, t_mini *attributes)//should this be an int function?
{
	int	i;
	int	newline;
//	int	output;
	int	saved_std;

	i = 1;
	newline = 1;
	saved_std = dup(STDOUT_FILENO);
	if (attributes->cmd_index > 1)
	{
		if (attributes->i < attributes->cmd_index)
		{
			dup2(attributes->pipe_arr[attributes->i - 1][WRITE], STDOUT_FILENO);
			close(attributes->pipe_arr[attributes->i - 1][WRITE]);
		}
		if (attributes->i > 1)
			close(attributes->pipe_arr[attributes->i - 2][READ]);
	}	
	if (node->outfile)
	{
		if (check_outfile(node, attributes))
		{
			attributes->exitcode = 1;
			return ;
		}
	}
	if (node->append)
	{
		if (check_append(node, attributes))
		{
			attributes->exitcode = 1;
			return ;
		}
	}
	if (node->cmd_arr[i] && ft_strcmp(node->cmd_arr[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (node->cmd_arr[i] != NULL)
	{
		printf("%s", node->cmd_arr[i]);
		if (node->cmd_arr[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	dup2(saved_std, STDOUT_FILENO);
	close(saved_std);
	attributes->exitcode = 0;
}

void	ft_cd(char **cmd_array, t_mini *attributes)//needs to handle edge cases and complicated relative paths
{
	char	*home;

	attributes->exitcode = 0;
	if (cmd_array[2] != NULL)
	{
		ft_putstr_fd(" too many arguments\n", 2);
		attributes->exitcode = 1;
		return ;
	}
	if (cmd_array[1] == NULL)
	{
		home = get_env_value(attributes, "HOME");
		if (!home || home[0] == '\0')
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			attributes->exitcode = 1;
			return ;
		}
		chdir(home);
	}
	else
	{
		if (chdir(cmd_array[1]) == -1)
		{	
			perror("cd");
			attributes->exitcode = 1;
		}
	}
}

void	handle_builtin(t_cmd_table *node, int flag, t_mini *attributes)
{
	if (flag == BUILTIN_ECHO)
		ft_echo(node, attributes);
	if (flag == BUILTIN_CD)
		ft_cd(node->cmd_arr, attributes);
	if (flag == BUILTIN_PWD)
		ft_pwd(attributes);
	if (flag == BUILTIN_EXPORT)
		ft_export(node->cmd_arr, attributes);
	if (flag == BUILTIN_UNSET)
		remove_env_var(node->cmd_arr, attributes);
	if (flag == BUILTIN_ENV)
		ft_env(node, attributes);
	if (flag == BUILTIN_EXIT && attributes->cmd_index == 1)
		ft_exit(node->cmd_arr, attributes);
	if (flag == BUILTIN_EXIT && attributes->cmd_index > 1)
		ft_child_exit(node->cmd_arr, attributes);
}

// checks if command is one of the builtins and which one
int	is_builtin(char *cmd_text)
{
	if (ft_strcmp(cmd_text, "echo") == 0)
		return (BUILTIN_ECHO);
	if (ft_strcmp(cmd_text, "cd") == 0)
		return (BUILTIN_CD);
	if (ft_strcmp(cmd_text, "pwd") == 0)
		return (BUILTIN_PWD);
	if (ft_strcmp(cmd_text, "export") == 0)
		return (BUILTIN_EXPORT);
	if (ft_strcmp(cmd_text, "unset") == 0)
		return (BUILTIN_UNSET);
	if (ft_strcmp(cmd_text, "env") == 0)
		return (BUILTIN_ENV);
	if (ft_strcmp(cmd_text, "exit") == 0)
		return (BUILTIN_EXIT);
	else
		return (0);
}
