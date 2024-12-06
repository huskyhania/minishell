/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envp_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:00:04 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/06 21:04:00 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_in_envp(char **envp, const char *key)
{
    size_t key_len;
    int i = 0;
    key_len = ft_strlen(key);
    while (envp[i] != NULL)
    {
	    if (ft_strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
		    return envp[i];
	    i++;
    }
    return NULL;
}

static int find_env_index(char **envp, const char *key)
{
    size_t key_len = ft_strlen(key);

    for (int i = 0; envp[i] != NULL; i++)
    {
        if (ft_strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
        {
            return i;
        }
    }
    return -1;
}

int	remove_env_var(char **cmd_array, char **envp)
{
	int	index;
	char	*key;

	key = cmd_array[1];
	index = find_env_index(envp, key);
	if (index < 0)//for error handling
		return (-1);
	while (envp[index + 1] != NULL)
	{
		envp[index] = envp[index + 1];
		index++;
	}
	envp[index] = NULL;
	ft_env(envp);//test for command line version
	return (0);
}
