/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 19:22:47 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/05 18:07:04 by hskrzypi         ###   ########.fr       */
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

char	*check_command(const char *cmd)
{
	if (access(cmd, X_OK) == 0)
	{
		//px->exitcode = 0;
		return (ft_strdup(cmd));
	}
	if (access(cmd, F_OK) == 0)
	{
		errno = EACCES;
		perror(cmd);
		//px->exitcode = 126;
		//display_error(px, cmd);
	}
	else
	{
		errno = ENOENT;
		//px->exitcode = 127;
		//display_error(px, cmd);
		perror(cmd);
	}
	return (NULL);
}

static int	check_access(const char *full_path, int *found)
{
	if (access(full_path, X_OK) == 0)
	{
		//px->exitcode = 0;
		return (1);
	}
	else if (access(full_path, F_OK) == 0)
	{
		*found = 1;
		return (0);
	}
	return (-1);
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

static char	*search_in_path(const char *cmd, char **directories)
{
	int		i;
	int		found;
	char	*result;

	i = 0;
	found = 0;
	while (directories[i] != NULL)
	{
		result = check_full_path(cmd, directories[i], &found);
		if (result)
		{
			//free_array(&directories);
			return (result);
		}
		i++;
	}
	//free_array(&directories);
	if (found)
	{
		perror("126");
		//set_error_and_display(126, px, cmd);
	}
	else
	{
		perror("127");
		//set_error_and_display(127, px, cmd);
	}
	return (NULL);
}

char	*get_command_path(const char *cmd, char **envp)
{
	char	*path_env;
	char	**directories;
	(void)envp;

	if (ft_strchr(cmd, '/'))
		return (check_command(cmd));
	path_env = find_path_in_envp(envp);
	if (!path_env)
	{
		errno = ENOENT;
		perror("cmd");
		//px->exitcode = 127;
		//display_error(px, cmd);
		return (NULL);
	}
	directories = ft_split(path_env, ':');
	if (!directories)
		return (NULL);
	return (search_in_path(cmd, directories));
}
