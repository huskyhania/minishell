/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envp_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:00:04 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/16 13:38:27 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	remove_env_var(char **cmd_array, t_mini *attributes)
{
	t_envp *current;
	
	if (!cmd_array[1] || !attributes || !attributes->envp_heap)
		return (-1); //exit code?
	current = attributes->envp_heap;
	while (current)
	{
		if (ft_strcmp(current->key, cmd_array[1]) == 0)
		{
			if (current->value)
				free(current->value);
			current->value = malloc(1);
			if (!current->value)
			{
				printf("malloc fail");
				return (-1);
			}
			current->value[0] = '\0';
			ft_env(attributes);
			return (0);
		}
		current = current->next;
	}
	ft_env(attributes);//test for command line version
	return (-1);
}

char	*get_env_value(t_mini *attributes, char *key)
{
	t_envp *current;

	if (!attributes || !attributes->envp_heap || !key)
		return (NULL);
	current = attributes->envp_heap;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}
