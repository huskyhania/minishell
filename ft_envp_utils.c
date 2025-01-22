/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envp_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:22:05 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/22 16:05:26 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	envp_list_size(t_envp *envp_heap)
{
	t_envp	*current;
	int		len;

	len = 0;
	current = envp_heap;
	while (current)
	{
		len++;
		current = current->next;
	}
	return (len);
}

char	*create_envp_entry(t_envp *envp_node)
{
	char	*entry;
	int		key_value_len;

	key_value_len = ft_strlen(envp_node->key) + ft_strlen(envp_node->value) + 2;
	entry = malloc(key_value_len);
	if (!entry)
	{
		ft_putstr_fd("malloc call fail\n", 2);
		return (NULL);
	}
	ft_strlcpy(entry, envp_node->key, ft_strlen(envp_node->key) + 1);
	ft_strlcat(entry, "=", key_value_len);
	ft_strlcat(entry, envp_node->value, key_value_len);
	return (entry);
}

void	free_envp_list(t_envp *envp_heap)
{
	t_envp	*temp;

	while (envp_heap)
	{
		temp = envp_heap;
		envp_heap = envp_heap->next;
		if (temp->key)
			free(temp->key);
		if (temp->value)
			free(temp->value);
		free(temp);
	}
}

void	print_envp_list(t_envp *envp_heap)
{
	t_envp	*helper;

	helper = envp_heap;
	while (helper)
	{
		if (helper->value)
		{
			printf("%s", helper->key);
			printf("=");
			printf("%s", helper->value);
			printf("\n");
		}
		helper = helper->next;
	}
}
