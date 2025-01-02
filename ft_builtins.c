/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:00:04 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/31 16:09:17 by hskrzypi         ###   ########.fr       */
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

void	ft_env(t_mini *attributes)
{
	char	*path_value;

	path_value = get_env_value(attributes, "PATH");
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
}

void	ft_echo(char **cmd_array, t_mini *attributes)//should this be an int function?
{
	//if no redirection
	int	fd;
	int	i;
	int	newline;

	fd = 1;
	i = 1;
	newline = 1;
	//else get fd from struct?
	if (cmd_array[i] && ft_strcmp(cmd_array[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (cmd_array[i] != NULL)
	{
		ft_putstr_fd(cmd_array[i], fd);
		if (cmd_array[i + 1] != NULL)
			ft_putstr_fd(" ", fd);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", fd);
	attributes->exitcode = 0;
}

void	ft_cd(char **cmd_array, t_mini *attributes)//needs to handle edge cases and complicated relative paths
{
	char	*home;

	attributes->exitcode = 0;
	if (cmd_array[2] != NULL)
	{
		printf("too many arguments for cd\n");
		attributes->exitcode = 1;
		return ;
	}
	if (cmd_array[1] == NULL)
	{
		home = get_env_value(attributes, "HOME");
		if (!home || home[0] == '\0')
		{
			printf("cd: HOME not set\n");
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

void	handle_builtin(char **cmd_array, int flag, t_mini *attributes)
{
	if (flag == BUILTIN_ECHO)
		ft_echo(cmd_array, attributes);
	if (flag == BUILTIN_CD)
		ft_cd(cmd_array, attributes);
	if (flag == BUILTIN_PWD)
		ft_pwd(attributes);
	if (flag == BUILTIN_EXPORT)
		ft_export(cmd_array, attributes);
	if (flag == BUILTIN_UNSET)
		remove_env_var(cmd_array, attributes);
	if (flag == BUILTIN_ENV)
		ft_env(attributes);
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
