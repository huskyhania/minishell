/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 19:22:47 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/15 00:55:35 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_paths(const char *dir, const char *cmd)
{
	size_t	dir_len;
	size_t	cmd_len;
	char	*full_path;

	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	full_path = malloc(dir_len + cmd_len + 2);
	if (!full_path)
		return (NULL);
	ft_strlcpy(full_path, dir, dir_len + 1);
	full_path[dir_len] = '/';
	ft_strlcpy(full_path + dir_len + 1, cmd, cmd_len + 1);
	return (full_path);
}

static char	*check_full_path(const char *cmd, const char *dir, int *found)
{
	char	*full_path;

	full_path = join_paths(dir, cmd);
	if (!full_path)
		return (NULL);
	if (check_access(full_path, found) == 1)
	{
		return (full_path);
	}
	free(full_path);
	return (NULL);
}

static char	*search_in_path(const char *cmd, char **dirs, t_mini *attributes)
{
	int		i;
	int		found;
	char	*result;

	i = 0;
	found = 0;
	while (dirs[i] != NULL)
	{
		result = check_full_path(cmd, dirs[i], &found);
		if (result)
		{
			free_array(dirs);
			return (result);
		}
		i++;
	}
	free_array(dirs);
	if (found)
		set_error_and_display(126, attributes, cmd);
	else
		set_error_and_display(127, attributes, cmd);
	return (NULL);
}

char	*search_in_cwd(const char *cmd, t_mini *attributes)
{
	char	*cwd;
	char	*cmd_path;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		syscall_fail(1, attributes, "cwd");
		return (NULL);
	}
	cmd_path = join_paths(cwd, cmd);
	free(cwd);
	if (cmd_path)
	{
		if (access(cmd_path, X_OK) == 0)
		{
			attributes->exitcode = 0;
			return (cmd_path);
		}
		free(cmd_path);
	}
	return (NULL);
}

char	*get_command_path(const char *cmd, t_mini *attributes)
{
	char	*path_env;
	char	*cmd_path;
	char	**directories;

	if (ft_strchr(cmd, '/'))
		return (check_command(cmd, attributes));
	path_env = get_env_value(attributes, "PATH");
	if (!path_env)
	{
		cmd_path = search_in_cwd(cmd, attributes);
		if (!cmd_path)
		{
			ft_putstr_fd((char *)cmd, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			attributes->exitcode = 127;
			return (NULL);
		}
		else
			return (cmd_path);
	}
	directories = ft_split(path_env, ':');
	if (!directories)
		return (NULL);
	return (search_in_path(cmd, directories, attributes));
}	
