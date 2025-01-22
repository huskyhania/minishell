/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ll_envp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 12:28:37 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/12 17:59:33 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envp	*find_last(t_envp *envp_heap)
{
	if (!envp_heap)
		return (NULL);
	while (envp_heap->next)
		envp_heap = envp_heap->next;
	return (envp_heap);
}

char	*get_key(char *s)
{
	int		i;
	char	*key;

	i = 0;
	while (s[i] != '+' && s[i] != '=' && s[i] != '\0')
		i++;
	key = malloc((i + 1) * sizeof(char));
	if (!key)
		return (NULL);
	i = 0;
	while (s[i] != '+' && s[i] != '=' && s[i] != '\0')
	{
		key[i] = s[i];
		i++;
	}
	key[i] = '\0';
	return (key);
}

char	*get_value(char *s)
{
	char	*value;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (s[i] && s[i] != '=')
		i++;
	if (s[i] == '=')
		i++;
	value = malloc((ft_strlen(s + i) + 1) * sizeof(char));
	if (!value)
		return (NULL);
	while (s[i] != '\0')
		value[j++] = s[i++];
	value[j] = '\0';
	return (value);
}

static void	append_node(t_envp **envp_heap, t_envp *node)
{
	t_envp	*last_node;

	if (!(*envp_heap))
		*envp_heap = node;
	else
	{
		last_node = find_last(*envp_heap);
		last_node->next = node;
		node->prev = last_node;
	}
}

t_envp	*create_node(char *s, t_envp **envp_heap)
{
	t_envp	*node;

	node = malloc(sizeof(t_envp));
	if (!node)
		return (NULL);
	node->key = get_key(s);
	if (ft_strchr(s, '='))
		node->value = get_value(s);
	else
		node->value = NULL;
	if (!node->key || (!node->value && ft_strchr(s, '=')))
	{
		printf("malloc error in evnp creation");
		if (node->key)
			free(node->key);
		free(node);
		return (NULL);
	}
	node->next = NULL;
	node->prev = NULL;
	append_node(envp_heap, node);
	return (node);
}
