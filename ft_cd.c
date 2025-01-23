/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 14:37:58 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/19 15:17:31 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_home(t_mini *attributes, char *old_pwd)
{
	char	*home;

	home = get_env_value(attributes, "HOME");
	if (!home || home[0] == '\0')
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		check_and_free_string(old_pwd);
		return (1);
	}
	if (chdir(home) == -1)
	{
		check_and_free_string(old_pwd);
		syscall_fail(1, attributes, "chdir");
		return (1);
	}
	return (0);
}

static int	update_oldpwd(t_envp **envp_heap, char *old_pwd)
{
	t_envp	*current;

	current = *envp_heap;
	if (!old_pwd || !envp_heap || !*envp_heap)
		return (1);
	while (current)
	{
		if (ft_strcmp("OLDPWD", current->key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(old_pwd);
			free(old_pwd);
			return (0);
		}
		current = current->next;
	}
	ft_putstr_fd("OLDPWD not found\n", 2);
	free(old_pwd);
	return (1);
}	

static void	cd_error(int flag, t_mini *attributes)
{
	if (flag == 1)
		ft_putstr_fd("cd: too many arguments\n", 2);
	if (flag == 2)
		ft_putstr_fd(" No such file or directory\n", 2);
	update_exitcode(1, attributes);
}

void	ft_cd(char **cmd_array, int i, t_mini *attributes)
{
	char	*old_pwd;

	while (cmd_array[i])
		i++;
	if (i > 2)
		return (cd_error(1, attributes));
	old_pwd = getcwd(NULL, 0);
	if (cmd_array[1] == NULL || (ft_strcmp(cmd_array[1], "--") == 0))
	{
		if (cd_home(attributes, old_pwd))
		{
			update_exitcode(1, attributes);
			return ;
		}
	}
	else
	{
		if (chdir(cmd_array[1]) == -1)
		{
			if (old_pwd)
				free(old_pwd);
			return (cd_error(2, attributes));
		}
	}
	update_oldpwd(&attributes->envp_heap, old_pwd);
}
