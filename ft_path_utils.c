/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_path_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 15:19:44 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/19 15:22:01 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_path_in_envp(char *envp[])
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

char	*check_command(const char *cmd, t_mini *attributes)
{
	struct stat	path_stat;

	if (stat(cmd, &path_stat) != 0)
	{
		update_exitcode(127, attributes);
		ft_putstr_fd((char *)cmd, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (NULL);
	}
	if (S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd((char *)cmd, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		update_exitcode(126, attributes);
		return (NULL);
	}
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	if (access(cmd, F_OK) == 0)
		set_error_and_display(126, attributes, cmd);
	return (NULL);
}

int	check_access(const char *full_path, int *found)
{
	if (access(full_path, X_OK) == 0)
		return (1);
	else if (access(full_path, F_OK) == 0)
	{
		*found = 1;
		return (0);
	}
	return (-1);
}
