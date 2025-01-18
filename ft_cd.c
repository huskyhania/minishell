/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 14:37:58 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/18 16:00:16 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_home(t_mini *attributes)
{
	char	*home;

	home = get_env_value(attributes, "HOME");
	if (!home || home[0] == '\0')
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		return (1);
	}
	return (chdir(home));
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

void	ft_cd(char **cmd_array, t_mini *attributes)
{
	int		i;
	char	*old_pwd;

	i = 0;
	while (cmd_array[i])
		i++;
	if (i > 2)
	{
		ft_putstr_fd(" too many arguments\n", 2);
		return (update_exitcode(1, attributes));
	}
	old_pwd = getcwd(NULL, 0);
	if (cmd_array[1] == NULL)
	{
		if (cd_home(attributes))
			return (syscall_fail(1, attributes, "chdir"));
	}
	else
	{
		if (chdir(cmd_array[1]) == -1)
		{
			ft_putstr_fd(" No such file or directory\n", 2);
			return(update_exitcode(1, attributes));
		}
	}
	update_oldpwd(&attributes->envp_heap, old_pwd);
}
