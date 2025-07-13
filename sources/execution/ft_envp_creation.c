/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envp_creation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:13:34 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/22 16:14:31 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**envp_to_array(t_envp *envp_heap)
{
	int		len;
	int		i;
	char	**envp_array;
	t_envp	*current;

	i = -1;
	len = envp_list_size(envp_heap);
	envp_array = malloc((len + 1) * sizeof(char *));
	if (!envp_array)
		return (NULL);
	current = envp_heap;
	while (++i < len)
	{
		envp_array[i] = create_envp_entry(current);
		if (!envp_array[i])
		{
			free_array(envp_array);
			return (NULL);
		}
		current = current->next;
	}
	envp_array[len] = NULL;
	return (envp_array);
}

t_envp	*envp_to_list(char **envp)
{
	int		i;
	t_envp	*envp_heap;

	envp_heap = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		if (!create_node(envp[i], &envp_heap))
		{
			free_envp_list(envp_heap);
			return (NULL);
		}
		i++;
	}
	return (envp_heap);
}
