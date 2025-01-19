/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envp_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:00:04 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/19 17:32:15 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	remove_env_var(char **cmd_array, t_mini *attributes)
{
	t_envp	*current;
	t_envp	*to_delete;

	if (!cmd_array[1] || !attributes || !attributes->envp_heap)
		return (-1);
	current = attributes->envp_heap;
	while (current)
	{
		if (ft_strcmp(current->key, cmd_array[1]) == 0)
		{
			to_delete = current;
			if (current->prev)
				current->prev->next = current->next;
			if (current->next)
				current->next->prev = current->prev;
			if (attributes->envp_heap == current)
				attributes->envp_heap = current->next;
			free(to_delete->key);
			free(to_delete->value);
			free(to_delete);
			return (0);
		}
		current = current->next;
	}
	return (-1);
}

char	*get_env_value(t_mini *attributes, char *key)
{
	t_envp	*current;

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
