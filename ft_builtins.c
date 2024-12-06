/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:00:04 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/06 20:31:32 by hskrzypi         ###   ########.fr       */
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

void	ft_env(char **envp_copy)
{
	int	i;

	i = 0;
	while (envp_copy[i] != NULL)
	{
		printf("%s\n", envp_copy[i]);
		i++;
	}
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

char	*find_home_in_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "HOME=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

void	ft_cd(char **cmd_array, char **envp_copy)//needs to handle edge cases and complicated relative paths
{
	char	*home;
	if (cmd_array[2] != NULL)
	{
		printf("too many arguments for cd\n");
		return ;
	}
	if (cmd_array[1] == NULL)
	{
		home = find_home_in_envp(envp_copy);
		if (!home)
		{
			printf("cd: HOME not set\n");
			return ;
		}
		chdir(home);
	}
	else 
		chdir(cmd_array[1]);
	//test below - this shouldn't be necessary for when minishell actually keeps running
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
    		printf("New directory: %s\n", cwd);
}

void	handle_builtin(char **cmd_array, int flag, char **envp_copy)
{

	(void)cmd_array;
	if (flag == 101)
		ft_echo(cmd_array);
	if (flag == 102)
		ft_cd(cmd_array, envp_copy);
	if (flag == 103)
		ft_pwd();
	if (flag == 105)
		remove_env_var(cmd_array, envp_copy);
	if (flag == 106)
		ft_env(envp_copy);
}

int	is_builtin(char *cmd_text)
{
	if (ft_strcmp(cmd_text, "echo") == 0)
		return (101);
	if (ft_strcmp(cmd_text, "cd") == 0)
		return (102);
	if (ft_strcmp(cmd_text, "pwd") == 0)
		return (103);
	if (ft_strcmp(cmd_text, "export") == 0)
		return (104);
	if (ft_strcmp(cmd_text, "unset") == 0)
		return (105);
	if (ft_strcmp(cmd_text, "env") == 0)
		return (106);
	if (ft_strcmp(cmd_text, "exit") == 0)
		return (107);
	else
		return (0);
}
