/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:00:04 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/16 13:29:46 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && (s1[i] != '\0' || s2[i] != '\0'))
		i++;
	return (s1[i] - s2[i]);
}

void	ft_pwd(void)
{
	char	*cwd;

	cwd = NULL;
	cwd = getcwd(NULL, 0);
	//add a malloc check
	printf("%s\n", cwd);
	free(cwd);
}

void	ft_env(t_mini *attributes)
{
	char	*path_value;

	path_value = get_env_value(attributes, "PATH");
	if (!path_value || path_value[0] == '\0')
	{
		printf("exit 127: No such file or directory");
		return ;
	}
	else
		print_envp_list(attributes->envp_heap);
}

void	ft_echo(char **cmd_array)//should this be an int function?
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
}

void	ft_cd(char **cmd_array, t_mini *attributes)//needs to handle edge cases and complicated relative paths
{
	char	*home;
	if (cmd_array[2] != NULL)
	{
		printf("too many arguments for cd\n");
		return ;
	}
	if (cmd_array[1] == NULL)
	{
		home = get_env_value(attributes, "HOME");
		if (!home || home[0] == '\0')
		{
			printf("cd: HOME not set\n");
			return ;
		}
		chdir(home);
	}
	else
	{
		if (chdir(cmd_array[1]) == -1)
			perror("cd");//needs to update exit code to 1
	}
	//test below - this shouldn't be necessary for when minishell actually keeps running
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
    		printf("New directory: %s\n", cwd);
}

void	handle_builtin(char **cmd_array, int flag, t_mini *attributes)
{
	if (flag == BUILTIN_ECHO)
		ft_echo(cmd_array);
	if (flag == BUILTIN_CD)
		ft_cd(cmd_array, attributes);
	if (flag == BUILTIN_PWD)
		ft_pwd();
	if (flag == BUILTIN_EXPORT)
		ft_export(cmd_array, attributes);
	if (flag == BUILTIN_UNSET)
		remove_env_var(cmd_array, attributes);
	if (flag == BUILTIN_ENV)
		ft_env(attributes);
}

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
