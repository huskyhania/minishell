/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cleanup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 16:49:52 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/31 17:20:39 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_envp_ll(t_envp **envp_heap)
{
	t_envp	*current;
	t_envp	*next_node;

	if (!envp_heap || !(*envp_heap))
		return ;
	current = *envp_heap;
	while (current)
	{
		next_node = current->next;
		if (current->key)
		{
			free(current->key);
			current->key = NULL;
		}
		if (current->value)
		{
			free(current->value);
			current->value = NULL;
		}
		free(current);
		current = next_node;
	}
	*envp_heap = NULL;
}

void	envp_cleanup(t_mini *attributes)
{
	if (attributes->envp_heap)
		clean_envp_ll(&attributes->envp_heap);
	if (attributes->envp_arr)
	{
		free_array(attributes->envp_arr);
		attributes->envp_arr = NULL;
	}
}
